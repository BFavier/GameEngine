#include <GameEngine/world/Model/Part.hpp>
#include <GameEngine/Engine.hpp>
using namespace GameEngine;

Part::Part()
{
    allocate();
}

Part::Part(const Part& other)
{
    *this = other;
    allocate();
}

Part::Part(Mesh* mesh)
{
    set_mesh(mesh);
    allocate();
}

Part::Part(Mesh* mesh, const Material& mat, const std::list<Texture>& texs)
{
    set_mesh(mesh);
    material = mat;
    textures = texs;
    allocate();
}

Part::Part(const std::string& model_path, const aiScene* aiscene, const aiMesh* aimesh)
{
    //Creating mesh
    _mesh = new Mesh(aimesh);
    //Reading material
    aiMaterial* aimaterial = aiscene->mMaterials[aimesh->mMaterialIndex];
    read_material(aimaterial);
    //Reading ambient textures
    read_textures(aiTextureType_AMBIENT, textures, model_path, aimaterial, aimesh);
    //Reading diffuse textures
    read_textures(aiTextureType_DIFFUSE, textures, model_path, aimaterial, aimesh);
    //Reading normal mapping textures
    read_textures(aiTextureType_NORMALS, normal_maps, model_path, aimaterial, aimesh);
    /*
    //Reading height mapping textures
    read_textures(aiTextureType_HEIGHT, normal_maps, model_path, aimaterial, aimesh);
    */
    //Reading specular textures
    read_textures(aiTextureType_SPECULAR, reflect_textures, model_path, aimaterial, aimesh);
    //Loading to GPU
    allocate();
}

Part::~Part()
{
    set_mesh(nullptr);
    deallocate();
}

void Part::upload()
{
    load_data(false);
}

Mesh* Part::get_mesh() const
{
    return _mesh;
}

unsigned int Part::ID() const
{
    return _ID;
}

void Part::set_mesh(Mesh* mesh)
{
    Mesh* previous = _mesh;
    _mesh = mesh;
    if (mesh != nullptr)
    {
        mesh->n_users ++;
    }
    if (previous != nullptr)
    {
        previous->n_users --;
        if (previous->n_users == 0)
        {
            delete previous;
        }
    }
}

Part& Part::operator=(const Part& other)
{
    name = other.name;
    textures = other.textures;
    material = other.material;
    set_mesh(other.get_mesh());
    return *this;
}

void Part::load_data(bool reallocate_buffer)
{
    //If the size changed reallocate the buffer
    if (textures.size() != _n_textures_GPU || reallocate_buffer)
    {
        unsigned int memory_size = sizeof(glm::vec4) + 2*textures.size()*sizeof(uint64_t);
        glNamedBufferData(_ID, memory_size, nullptr, GL_DYNAMIC_READ);
        _n_textures_GPU = textures.size();
    }
    unsigned int offset = 0;
    unsigned int memory_size = 0;
    //storing the material color
    glm::vec4 clr(material.color.R/255., material.color.G/255., material.color.B/255., material.color.A/255.);
    memory_size = sizeof(glm::vec4);
    glNamedBufferSubData(_ID, offset, memory_size, &clr);
    offset += memory_size;
    //storing the texture data
    std::vector<uint64_t> data(2*textures.size());
    memory_size = data.size()*sizeof(uint64_t);
    unsigned int i = 0;
    for (Texture& tex : textures)
    {
        if (not(tex.complete()))
        {
            THROW_ERROR("Tried to upload a Part containing an incomplete texture (no Image or no UVmap)")
        }
        data[i] = tex.get_UVmap()->handle();
        data[i+1] = tex.get_image()->handle();
        i += 2;
    }
    glNamedBufferSubData(_ID, offset, data.size()*sizeof(uint64_t), &data[0]);
}

void Part::allocate()
{
    Engine::initialize();
    deallocate();
    glCreateBuffers(1, &_ID);
    load_data(true);
}

void Part::deallocate()
{
    glDeleteBuffers(1, &_ID);
}

void Part::read_material(aiMaterial* aimaterial)
{
    aiColor3D color;
    //color
    if (aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
    {
        material.color.R = 255.*color.r;
        material.color.G = 255.*color.g;
        material.color.B = 255.*color.b;
    }
    if (aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)//If ambient color exists overwrite diffuse color
    {
        material.color.R = color.r;
        material.color.G = color.g;
        material.color.B = color.b;
    }
    float opacity;
    if (aimaterial->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS)
    {
        material.color.A = opacity;
    }
    //color_reflects
    if (aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
    {
        material.color_reflects.R = color.r;
        material.color_reflects.G = color.g;
        material.color_reflects.B = color.b;
    }
    //shininess
    float shininess;
    if (aimaterial->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
    {
        material.shininess = static_cast<double>(shininess);
    }
    //two_sided
    int twosided = 0;
    if (aimaterial->Get(AI_MATKEY_TWOSIDED, twosided) == AI_SUCCESS)
    {
        material.two_sided = static_cast<bool>(twosided);
    }
    //name
    aiString mat_name;
    if (aimaterial->Get(AI_MATKEY_NAME, mat_name) == AI_SUCCESS)
    {
        name = mat_name.C_Str();
    }
}

void Part::read_textures(aiTextureType type, std::list<Texture>& destination, const std::string& model_dir, aiMaterial* aimaterial, const aiMesh* aimesh) const
{
    unsigned int n_textures = aimaterial->GetTextureCount(type);
    for(unsigned int i=0; i<n_textures; i++)
    {
        unsigned int UVindex;
        aiString ai_str;
        aimaterial->GetTexture(type, i, &ai_str, nullptr, &UVindex);
        std::vector<UV> UVs;
        for (unsigned int i=0; i<aimesh->mNumVertices; i++)
        {
            const aiVector3D& uv = aimesh->mTextureCoords[UVindex][i];
            UVs.emplace_back(uv.x, uv.y);
        }
        std::string texture_path = model_dir+"/"+std::string(ai_str.C_Str());
        destination.emplace_back(texture_path, UVs);
    }
}
