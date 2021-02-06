#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/graphics/Image/Image.hpp>
#include "UVmap.hpp"

namespace GameEngine
{
    class Texture
    {
    public:
        Texture();
        Texture(const Texture& other);
        Texture(const std::string& image_path, const std::vector<UV>& UVs);
        Texture(Image* image, UVmap* uv_map);
        ~Texture();
    public:
        Texture deep_copy() const;
        bool complete() const;
        UVmap* get_UVmap() const;
        void set_UVmap(UVmap* uv_map);
        Image* get_image() const;
        void set_image(Image* image);
        Texture& operator=(const Texture& other);
        //static Texture heightmap_to_normalmap(const Texture& other);
    protected:
        UVmap* _uv_map = nullptr;
        Image* _image = nullptr;
    };
}
