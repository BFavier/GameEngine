#include <GameEngine/graphics/Image/LayeredData.hpp>
using namespace GameEngine;

LayeredData::LayeredData()
{
    Engine::initialize();
    allocate();
    upload();
}

LayeredData::LayeredData(unsigned int width, unsigned int height, unsigned int depth)
{
    Engine::initialize();
    _width = width;
    _height = height;
    _depth = depth;
    data.resize(width*height*depth);
    std::fill(data.begin(), data.end(), 0.);
    allocate();
    upload();
}

LayeredData::~LayeredData()
{
    deallocate();
}

void LayeredData::clear()
{
    //GPU side fill
    float float_fill = 0.;
    glClearTexImage(_ID, 0, GL_RED, GL_FLOAT, &float_fill);
    //CPU side fill
    std::fill(data.begin(), data.end(), 0.);
}

double& LayeredData::at(unsigned int x, unsigned int y, unsigned int z)
{
    return data[x + y*_width + z*_width*_height];
}

unsigned int LayeredData::width() const
{
    return _width;
}

unsigned int LayeredData::height() const
{
    return _height;
}

unsigned int LayeredData::depth() const
{
    return _depth;
}

void LayeredData::resize(unsigned int width, unsigned int height, unsigned int depth)
{
    if (width != _width || height != _height || depth != _depth)
    {
        data.resize(width*height*depth);
        _width = width;
        _height = height;
        _depth = depth;
        deallocate();
        allocate();
    }
}

void LayeredData::upload() const
{
    if (!allocated())
    {
        THROW_ERROR("Cannot upload the LayeredData's data to GPU side if the GPU memory is not allocated. Call allocate() first.")
    }
    if (data.size() != _width*_height*_depth)
    {
        THROW_ERROR("Upload of LayeredData to GPU side failed: The size of the data on CPU side doesn't match the width, height and depth of the LayeredData.")
    }
    std::vector<float> float_data(data.size());
    for (unsigned int i=0; i<data.size(); i++)
    {
        float_data[i] = static_cast<float>(data[i]);
    }
    glTextureSubImage3D(_ID, 0, 0, 0, 0, _width, _height, _depth, GL_RED, GL_FLOAT, &float_data[0]);
}

void LayeredData::download()
{
    data.resize(_width*_height*_depth);
    std::vector<float> float_data(_width*_height*_depth);
    glGetTextureImage(_ID, 0, GL_RED, GL_FLOAT, _width*_height*_depth*sizeof(float), &float_data[0]);
    for (unsigned int x=0; x<_width; x++)
    {
        for (unsigned int y=0; y<_height; y++)
        {
            for (unsigned int z=0; z<_depth; z++)
            {
                at(x, y, z) = static_cast<double>(float_data[x + y*_width + z*_width*_height]);
            }
        }
    }
}

LayeredData& LayeredData::operator=(const LayeredData& other)
{
    resize(other.width(), other.height(), other.depth());
    glCopyImageSubData(other.ID(), GL_TEXTURE_3D, 0, 0, 0, 0,
                       _ID, GL_TEXTURE_3D, 0, 0, 0, 0,
                       _width, _height, _depth);
    return *this;
}

unsigned int LayeredData::ID() const
{
    return _ID;
}

uint64_t LayeredData::handle() const
{
    return _handle;
}

void LayeredData::allocate()
{
    if (allocated())
    {
        THROW_ERROR("Cannot allocate an LayeredData object that is already allocated.")
    }
    if (_width==0 || _height==0 || _depth==0)
    {
        THROW_ERROR("Layered data dimensions must be superior to 0")
    }
    //Create the new ID
    glCreateTextures(GL_TEXTURE_3D, 1, &_ID);
    //Allocate the VRAM on GPU
    glTextureStorage3D(_ID, 1, GL_R32F, _width, _height, _depth);
    //Generate the handle and make the texture resident
    _handle = glGetImageHandleARB(_ID, 0, true, 0, GL_R32F);
    glMakeImageHandleResidentARB(_handle, GL_READ_WRITE);
}

void LayeredData::deallocate()
{
    if (allocated())
    {
        if (glIsImageHandleResidentARB(_handle))
        {
            glMakeImageHandleNonResidentARB(_handle);
        }
        glDeleteTextures(1, &_ID);
        _ID = 0;
        _handle = 0;
    }
}

bool LayeredData::allocated() const
{
    return (_ID != 0);
}
