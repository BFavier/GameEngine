#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/world/Entity.hpp>
#include "Part.hpp"

namespace GameEngine
{
    class Model : public Entity
    {
        public:
            Model();
            Model(const Model& other);
            Model(const std::list<Part>& parts);
            Model(const std::string& model_path);
            ~Model();
        public:
            Model& operator=(const Model& other);
        public:
            std::string name = "model";
            std::list<Part> parts;
        private:
            void read_parts_recursively(const aiNode* node, const aiScene* scene, const std::string& model_dir);
    };
}
