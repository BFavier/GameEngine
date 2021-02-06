#include <GameEngine/world/Model/Texture.hpp>
using namespace GameEngine;

Texture::Texture()
{
}

Texture::Texture(const Texture& other)
{
    *this = other;
}

Texture::Texture(const std::string& image_path, const std::vector<UV>& uvs)
{
    _image = new Image(image_path);
    _uv_map = new UVmap(uvs);
}

Texture::Texture(Image* image, UVmap* uv_map)
{
    set_image(image);
    set_UVmap(uv_map);
}

Texture::~Texture()
{
    set_image(nullptr);
    set_UVmap(nullptr);
}

Texture Texture::deep_copy() const
{
    UVmap* new_UVmap = nullptr;
    if (_uv_map != nullptr)
    {
        new_UVmap = new UVmap(*_uv_map);
    }
    Image* new_image = nullptr;
    if (_image != nullptr)
    {
        new_image = new Image(*_image);
    }
    Texture texture;
    texture.set_image(new_image);
    texture.set_UVmap(new_UVmap);
    return texture;
}

bool Texture::complete() const
{
    return (_image != nullptr && _uv_map != nullptr);
}

UVmap* Texture::get_UVmap() const
{
    return _uv_map;
}

void Texture::set_UVmap(UVmap* uv_map)
{
    UVmap* previous = _uv_map;
    _uv_map = uv_map;
    if (uv_map != nullptr)
    {
        uv_map->n_users ++;
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

Image* Texture::get_image() const
{
    return _image;
}

void Texture::set_image(Image* image)
{
    Image* previous = _image;
    _image = image;
    if (image != nullptr)
    {
        image->n_users ++;
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

Texture& Texture::operator=(const Texture& other)
{
    set_UVmap(other.get_UVmap());
    set_image(other.get_image());
    return *this;
}
