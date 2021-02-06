#pragma once
#include <GameEngine/graphics/RGBA.hpp>
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Material.hpp"

namespace GameEngine
{
    class Part
    {
        public:
            Part();
            Part(const Part& other);
            Part(Mesh* mesh);
            Part(Mesh* mesh, const Material& material, const std::list<Texture>& textures);
            Part(const std::string& model_path, const aiScene* aiscene, const aiMesh* aimesh);
            ~Part();
        public:
            void upload();
            unsigned int ID() const;
            Mesh* get_mesh() const;
            void set_mesh(Mesh* mesh);
            Part& operator=(const Part& other);
        public:
            std::string name = "part";
            Material material;
            std::list<Texture> textures;
            std::list<Texture> normal_maps;
            std::list<Texture> reflect_textures;
        protected:
            Mesh* _mesh = nullptr;
            unsigned int _ID = 0;
            unsigned int _n_textures_GPU = 0;
        protected:
            void load_data(bool reallocate_buffer);
            void allocate();
            void deallocate();
            ///< Load the material from Assimp data structure
            void read_material(aiMaterial* aimaterial);
            ///< Load all the texture of the given type from an Assimp data structure
            void read_textures(aiTextureType type, std::list<Texture>& destination, const std::string& model_dir, aiMaterial* aimaterial, const aiMesh* aimesh) const;
    };
}
