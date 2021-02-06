#include <GameEngine/world/Model/UVmap.hpp>
#include <GameEngine/Engine.hpp>
using namespace GameEngine;

UVmap::UVmap()
{
    Engine::initialize();
    allocate();
    upload();
}

UVmap::UVmap(const UVmap& other)
{
    Engine::initialize();
    *this = other;
}

UVmap::UVmap(const std::initializer_list<UV>& uvs)
{
    Engine::initialize();
    UVs = uvs;
    allocate();
    upload();
}

UVmap::UVmap(const std::vector<UV>& uvs)
{
    Engine::initialize();
    UVs = uvs;
    allocate();
    upload();
}

UVmap::~UVmap()
{
    deallocate();
}

unsigned int UVmap::size() const
{
    return _size;
}

void UVmap::upload()
{
    if (!allocated())
    {
        THROW_ERROR("Cannot upload the image data to GPU side if the GPU memory is not allocated. Call allocate() first.")
    }
    if (UVs.size() != _size)
    {
        deallocate();
        allocate();
    }
    //copy data to GPU
    std::vector<float> data(2*_size);
    for (unsigned int i=0; i<_size; i++)
    {
        data[i] = UVs[i].u;
        data[_size+i] = UVs[i].v;
    }
    glTextureSubImage2D(_ID, 0, 0, 0, _size, 2, GL_RED, GL_FLOAT, &data[0]);
}

unsigned int UVmap::ID() const
{
    return _ID;
}

uint64_t UVmap::handle() const
{
    return _handle;
}

void UVmap::allocate()
{
    if (allocated())
    {
        THROW_ERROR("Cannot allocate an UVmap object that is already allocated.")
    }
    _size = UVs.size();
    //Create the new ID
    glCreateTextures(GL_TEXTURE_2D, 1, &_ID);
    //Allocate the VRAM on GPU
    glTextureStorage2D(_ID, 1, GL_R32F, _size, 2);
    //Generate the handle and makes the texture resident
    _handle = glGetTextureHandleARB(_ID);
    glMakeTextureHandleResidentARB(_handle);
}

void UVmap::deallocate()
{
    if (allocated())
    {
        if (glIsTextureHandleResidentARB(_handle))
        {
            glMakeTextureHandleNonResidentARB(_handle);
        }
        glDeleteTextures(1, &_ID);
        _ID = 0;
        _handle = 0;
    }
}

bool UVmap::allocated() const
{
    return (_ID != 0);
}

UVmap& UVmap::operator=(const UVmap& other)
{
    //Copy GPU side data
    if (other.allocated())
    {
        if (_size != other.size())
        {
            _size = other.size();
            deallocate();
            allocate();
        }
        glCopyImageSubData(other.ID(), GL_TEXTURE_2D, 0, 0, 0, 0,
                           _ID, GL_TEXTURE_2D, 0, 0, 0, 0,
                           _size, 2, 1);
    }
    else
    {
        deallocate();
    }
    //Copy CPU side data
    UVs = other.UVs;
    //return reference
    return *this;
}
