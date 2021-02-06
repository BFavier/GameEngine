#include <GameEngine/world/Model/Model.hpp>
using namespace GameEngine;

Model::Model()
{
    name = "model";
}

Model::Model(const Model& other) : Entity(other)
{
    *this = other;
}

Model::Model(const std::list<Part>& p)
{
    name = "model";
    parts = p;
}

Model::Model(const std::string& model_path)
{
    name = model_path;
    //Import the model file
    Assimp::Importer import;
    unsigned int options = (aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals |
                            aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph |
                            aiProcess_ValidateDataStructure | aiProcess_GenUVCoords);
    const aiScene* scene = import.ReadFile(model_path, options);
    //const aiScene* scene = import.ReadFile(model_path, 0);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        THROW_ERROR("Failed to load the model from path '"+model_path+"' : "+import.GetErrorString())
    }
    std::string model_dir = model_path.substr(0, model_path.find_last_of('/'));
    read_parts_recursively(scene->mRootNode, scene, model_dir);
}

Model::~Model()
{
}

Model& Model::operator=(const Model& other)
{
    Entity::operator=(other);
    parts = other.parts;
    return *this;
}

void Model::read_parts_recursively(const aiNode* node, const aiScene* scene, const std::string& model_dir)
{
    // process all the Node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
        parts.emplace_back(model_dir, scene, aimesh);
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        read_parts_recursively(node->mChildren[i], scene, model_dir);
    }
}
