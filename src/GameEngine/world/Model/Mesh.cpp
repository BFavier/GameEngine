#include <GameEngine/world/Model/Mesh.hpp>
#include <GameEngine/world/Model/Vertex.hpp>
#include <GameEngine/Engine.hpp>
using namespace GameEngine;

Mesh::Mesh()
{
    allocate();
}

Mesh::Mesh(const Mesh& other)
{
    *this = other;
    allocate();
}

Mesh::Mesh(const std::initializer_list<Face>& elements)
{
    std::vector<Face> data = elements;
    copy_faces(data);
    allocate();
}

Mesh::Mesh(const std::vector<Face>& faces, bool dyn)
{
    _dynamic = dyn;
    copy_faces(faces);
    allocate();
}

Mesh::Mesh(const aiMesh* aimesh)
{
    unsigned int n_colors = aimesh->GetNumColorChannels();
    //Loading the vertices
    vertices.reserve(aimesh->mNumVertices);
    for(unsigned int i = 0; i < aimesh->mNumVertices; i++)
    {
        //position
        if (!aimesh->HasPositions())
        {
            THROW_ERROR("The mesh contains no position for the vertices")
        }
        aiVector3D& pos = aimesh->mVertices[i];
        Vector position(pos.x, pos.y, pos.z);
        //normal
        Vector normal(0., 0., 0.);
        if (aimesh->HasNormals())
        {
            aiVector3D& norm = aimesh->mNormals[i];
            normal = {norm.x, norm.y, norm.z};
        }
        //tangent
        Vector tangent(0., 0., 0.);
        if (aimesh->HasTangentsAndBitangents())
        {
            aiVector3D& tan = aimesh->mTangents[i];
            tangent = {tan.x, tan.y, tan.z};
        }
        //color
        RGBA color(0, 0, 0, 0);
        for (unsigned int j=0; j<n_colors; j++)
        {
            aiColor4D& c = aimesh->mColors[j][i];
            color += RGBA(c.r, c.g, c.b, c.a);
        }
        vertices.emplace_back(position, normal, color);
    }
    //Loading the faces
    faces.reserve(aimesh->mNumFaces);
    for (unsigned int i=0; i < aimesh->mNumFaces; i++)
    {
        aiFace ai_face = aimesh->mFaces[i];
        //If the face is not triangular
        if (ai_face.mNumIndices != 3)
        {
            THROW_ERROR("When loading a model, found a face with "
                        +std::to_string(ai_face.mNumIndices)
                        +" vertices. Expected triangular faces with 3 vertices.")
        }
        unsigned int i1 = ai_face.mIndices[0];
        unsigned int i2 = ai_face.mIndices[1];
        unsigned int i3 = ai_face.mIndices[2];
        faces.emplace_back(vertices[i1], vertices[i2], vertices[i3]);
    }
    //Allocating on GPU memory
    allocate();
}

Mesh::~Mesh()
{
    deallocate();
    clear_VAOs();
}

void Mesh::upload()
{
    load_data(false);
}

bool Mesh::dynamic() const
{
    return _dynamic;
}

void Mesh::dynamic(bool enabled)
{
    if (_dynamic == enabled)
    {
        return;
    }
    _dynamic = enabled;
    load_data(true);
}

void Mesh::clear()
{
    faces.clear();
    vertices.clear();
}

void Mesh::copy_faces(const std::vector<Face>& copied_faces)
{
    clear();
    //index vertices
    for (const Face& face : copied_faces)
    {
        for (Vertex* v : face.vertices)
        {
            *v->index = std::numeric_limits<unsigned int>::max();
        }
    }
    unsigned int k=0;
    for (const Face& face : copied_faces)
    {
        for (Vertex* v : face.vertices)
        {
            if (*v->index == std::numeric_limits<unsigned int>::max())
            {
                *v->index = k;
                k++;
            }
        }
    }
    vertices.resize(k);
    for (const Face& face : copied_faces)
    {
        const Vertex& v1 = *face.vertices[0];
        const Vertex& v2 = *face.vertices[1];
        const Vertex& v3 = *face.vertices[2];
        unsigned int u1 = *v1.index;
        vertices[u1] = v1;
        unsigned int u2 = *v2.index;
        vertices[u2] = v2;
        unsigned int u3 = *v3.index;
        vertices[u3] = v3;
        faces.emplace_back(vertices[u1], vertices[u2], vertices[u3]);
    }
}

void Mesh::reindex_vertices() const
{
    unsigned int k=0;
    for (const Vertex& v : vertices)
    {
        *v.index = k;
        k++;
    }
}

unsigned int Mesh::VAO()
{
    OpenGLContext context = Engine::active_context();
    std::list<std::pair<OpenGLContext, unsigned int>>::iterator it = _VAOs.begin();
    while (it != _VAOs.end())
    {
        if (it->first == context)
        {
            return it->second;
        }
        if (std::find(Engine::contexts.begin(), Engine::contexts.end(), it->first) == Engine::contexts.end())
        {
            it = _VAOs.erase(it);
        }
        else
        {
            it++;
        }
    }
    unsigned int new_VAO = generate_VAO();
    _VAOs.push_back(std::make_pair(context, new_VAO));
    return new_VAO;
}

unsigned int Mesh::n_vertices_GPU() const
{
    return _n_vertices_GPU;
}

Mesh& Mesh::operator=(const Mesh& other)
{
    other.reindex_vertices();
    vertices = other.vertices;
    for (const Face& face : other.faces)
    {
        Vertex& v1 = vertices[*face.vertices[0]->index];
        Vertex& v2 = vertices[*face.vertices[1]->index];
        Vertex& v3 = vertices[*face.vertices[2]->index];
        faces.emplace_back(v1, v2, v3);
    }
    _dynamic = other.dynamic();
    return *this;
}

void Mesh::load_data(bool reallocate_buffers)
{
    //Vertex Buffer Object
    {
        //storing the data and setting the index of the vertices
        std::vector<GPUvertex> data(vertices.size());
        unsigned int i=0;
        for (Vertex& v : vertices)
        {
            data[i] = v.data();
            *v.index = i;
            i++;
        }
        //If the size changed reallocate the buffer, otherwise fills it
        if (vertices.size() != _n_vertices_GPU || reallocate_buffers)
        {
            glNamedBufferData(_VBO, data.size()*sizeof(GPUvertex), &data[0], (_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
            _n_vertices_GPU = vertices.size();
        }
        else
        {
            glNamedBufferSubData(_VBO, 0, data.size()*sizeof(GPUvertex), &data[0]);
        }
    }
    //Element Buffer Object
    {
        //storing the data
        std::vector<unsigned int> data(3*faces.size());
        unsigned int i=0;
        for (Face& face : faces)
        {
            if (not(face.valid()))
            {
                THROW_ERROR("One face of the mesh is missing some vertices")
            }
            data[i] = *face.vertices[0]->index;
            data[i+1] = *face.vertices[1]->index;
            data[i+2] = *face.vertices[2]->index;
            i += 3;
        }
        //If the size changed reallocate the buffer, otherwise fills it
        if (faces.size() != _n_faces_GPU || reallocate_buffers)
        {
            glNamedBufferData(_EBO, data.size()*sizeof(unsigned int), &data[0], (_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
            _n_faces_GPU = faces.size();
        }
        else
        {
            glNamedBufferSubData(_EBO, 0., data.size()*sizeof(unsigned int), &data[0]);
        }
    }
}

void Mesh::allocate()
{
    Engine::initialize();
    //deallocate
    deallocate();
    //Generate buffers
    glCreateBuffers(1, &_VBO);
    glCreateBuffers(1, &_EBO);
    //load data to Vertex Buffer Object and Element Buffer Object
    load_data(true);
}

void Mesh::deallocate()
{
    for (std::pair<const GLFWwindow*, unsigned int> VAO : _VAOs)
    {
        unsigned int vao = VAO.second;
        glDeleteVertexArrays(1, &vao);
    }
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
}

unsigned int Mesh::generate_VAO() const
{
    unsigned int VAO;
    //Generate Vertex Array Object
    glCreateVertexArrays(1, &VAO);
    //define the layout of data
    //      vertex position
    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayVertexBuffer(VAO, 0, _VBO, offsetof(GPUvertex, position), sizeof(GPUvertex));
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);
    //      vertex normal
    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayVertexBuffer(VAO, 1, _VBO, offsetof(GPUvertex, normal), sizeof(GPUvertex));
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 1, 1);
    //      vertex color
    glEnableVertexArrayAttrib(VAO, 2);
    glVertexArrayVertexBuffer(VAO, 2, _VBO, offsetof(GPUvertex, color), sizeof(GPUvertex));
    glVertexArrayAttribFormat(VAO, 2, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 2, 2);
    //Bind the Element Buffer Object to the Vertex Array Object
    glVertexArrayElementBuffer(VAO, _EBO);
    //Return the created VAO
    return VAO;
}

void Mesh::clear_VAOs()
{
    for (std::pair<OpenGLContext, unsigned int>& elements : _VAOs)
    {
        glDeleteVertexArrays(1, &elements.second);
    }
    _VAOs.clear();
}
