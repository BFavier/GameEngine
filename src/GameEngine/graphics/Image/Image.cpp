#include <GameEngine/graphics/Image/Image.hpp>
#include <GameEngine/world/Model/Model.hpp>
#include <GameEngine/graphics/shaders/shaders.hpp>
using namespace GameEngine;

Image::Image()
{
    Engine::initialize();
    allocate();
}

Image::Image(const Image& other)
{
    Engine::initialize();
    *this = other;
}

Image::Image(const std::string& path)
{
    Engine::initialize();
    //Loading data from file
    int width, height, n_channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &n_channels, 0);
    if (data == nullptr)
    {
        THROW_ERROR("Failed to load image from path '"+path+"'")
    }
    //Saving data
    _width = static_cast<unsigned int>(width);
    _height = static_cast<unsigned int>(height);
    pixels.resize(_width*_height);
    RGBA pxl;
    int k=0;
    for (unsigned int y=0; y<_height; y++)
    {
        for (unsigned int x=0; x<_width; x++)
        {
            if (n_channels == 1)
            {
                pxl.R = data[k];
                pxl.G = data[k];
                pxl.B = data[k];
                pxl.A = 255;
                k++;
            }
            else if (n_channels == 3)
            {
                pxl.R = data[k];
                pxl.G = data[k+1];
                pxl.B = data[k+2];
                pxl.A = 255;
                k += 3;
            }
            else if (n_channels == 4)
            {
                pxl.R = data[k];
                pxl.G = data[k+1];
                pxl.B = data[k+2];
                pxl.A = data[k+3];
                k += 4;
            }
            pixel(x, _height-1-y) = pxl;
        }
    }
    stbi_image_free(data);
    //allocating GPU space
    allocate();
    upload();
}

Image::Image(unsigned int width, unsigned int height, const RGBA& color)
{
    Engine::initialize();
    _FBOs = new std::list<std::pair<OpenGLContext, unsigned int>>();
    _width = width;
    _height = height;
    pixels.resize(_width*_height);
    std::fill(pixels.begin(), pixels.end(), color);
    allocate();
    upload();
}

Image::Image(unsigned int width, unsigned int height, const std::vector<RGBA>& pxl)
{
    Engine::initialize();
    pixels = pxl;
    _width = width;
    _height = height;
    allocate();
    upload();
}

Image::~Image()
{
    deallocate();
}

/*
void Image::draw(const Model& model, const Camera& camera)
{
    unsigned int fbo = _AA_surface.FBO();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    Engine::shader3D->draw(model, camera, width(), height(), _layers);
    glBlitNamedFramebuffer(fbo, FBO(),
                           0, 0, _AA_surface.width(), _AA_surface.height(),
                           0, 0, _width, _height,
                           GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glMemoryBarrier(GL_FRAMEBUFFER_BARRIER_BIT);
}
*/

void Image::save(const std::string& file_path)
{
    int img_width = static_cast<int>(_width);
    int img_height = static_cast<int>(_height);
    if (_width*_height != pixels.size())
    {
        THROW_ERROR("Failed to save image: The length of the data on CPU doesn't match the width and height of the image.")
    }
    std::string extension = Utilities::to_upper(Utilities::extension(file_path));
    int flag = 0;
    stbi_flip_vertically_on_write(1);
    std::vector<unsigned char> data(4*pixels.size());
    for (unsigned int i=0; i<pixels.size(); i++)
    {
        data[4*i+0] = pixels[i].R;
        data[4*i+1] = pixels[i].G;
        data[4*i+2] = pixels[i].B;
        data[4*i+3] = pixels[i].A;
    }
    if (extension == "PNG")
    {
        flag = stbi_write_png(file_path.c_str(), img_width, img_height, 4, &data[0], img_width*4);
    }
    else if (extension == "BMP")
    {
        //Alpha is not supported
        for (unsigned int i=0; i<pixels.size(); i++)
        {
            data[4*i+3] = 255;
        }
        flag = stbi_write_bmp(file_path.c_str(), img_width, img_height, 4, &data[0]);
    }
    else if (extension == "JPG")
    {
        flag = stbi_write_jpg(file_path.c_str(), img_width, img_height, 4, &data[0], 100);
    }
    else if (extension == "TGA")
    {
        flag = stbi_write_tga(file_path.c_str(), img_width, img_height, 4, &data[0]);
    }
    else
    {
        THROW_ERROR("Failed to save image: Unsupported extension '"+extension+"'")
    }
    //Checking the flag
    if (flag == 0)
    {
        THROW_ERROR("Failed to save image as '"+file_path+"': The path might not exist or you might not have write permission in this directory.");
    }
}

RGBA& Image::pixel(unsigned int x, unsigned int y)
{
    if (y > _height-1 || x > _width-1)
    {
        std::string err = "Pixel at position (x="+std::to_string(x)+", y="+std::to_string(y)
                            +") is out of range for image '"+name+"' with width="+std::to_string(_width)+" and height="
                            +std::to_string(_height);
        THROW_ERROR(err)
    }
    return pixels[y*_width+x];
}

void Image::clear(const RGBA& color)
{
    //GPU side fill
    std::vector<unsigned char> data = {color.R, color.G, color.B, color.A};
    glClearTexImage(_ID, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
    //CPU side fill
    std::fill(pixels.begin(), pixels.end(), color);
    //not up to date
}

void Image::resize(unsigned int width, unsigned int height)
{
    if (width != _width || height != _height)
    {
        pixels.resize(width*height);
        _width = width;
        _height = height;
        deallocate();
        allocate();
    }
}

unsigned int Image::width() const
{
    return _width;
}

unsigned int Image::height() const
{
    return _height;
}

unsigned int Image::transparency_layers() const
{
    return (_layers.depth() - 8)/12;
}

void Image::transparency_layers(unsigned int number)
{
    _layers.resize(_width, _height, 8+12*number);
}

unsigned int Image::AA() const
{
    return _AA_surface.AA();
}

void Image::AA(unsigned int samples)
{
    _AA_surface.AA(samples);
}

void Image::upload() const
{
    if (!allocated())
    {
        THROW_ERROR("Cannot upload the image data to GPU side if the GPU memory is not allocated. Call allocate() first.")
    }
    if (pixels.size() != _width*_height)
    {
        THROW_ERROR("Upload of image to GPU side failed: The size of the data on CPU side doesn't match the width and height of the image.")
    }
    std::vector<unsigned char> data(4*pixels.size());
    for (unsigned int i=0; i<pixels.size(); i++)
    {
        data[4*i+0] = pixels[i].R;
        data[4*i+1] = pixels[i].G;
        data[4*i+2] = pixels[i].B;
        data[4*i+3] = pixels[i].A;
    }
    glTextureSubImage2D(_ID, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
    //glGenerateTextureMipmap(_ID);
}

void Image::download()
{
    pixels.resize(_width*_height);
    std::vector<float> data(4*pixels.size());
    glGetTextureImage(_ID, 0, GL_RGBA, GL_FLOAT, _width*_height*4*sizeof(float), &data[0]);
    for (unsigned int i=0; i<pixels.size(); i++)
    {
        pixels[i].R = std::round(data[4*i+0]*255.f);
        pixels[i].G = std::round(data[4*i+1]*255.f);
        pixels[i].B = std::round(data[4*i+2]*255.f);
        pixels[i].A = std::round(data[4*i+3]*255.f);
    }
}

void Image::allocate()
{
    if (allocated())
    {
        THROW_ERROR("Cannot allocate an Image object that is already allocated.")
    }
    //Create the FBO list
    _FBOs = new std::list<std::pair<OpenGLContext, unsigned int>>();
    //  Allocate the GL_TEXTURE
    glCreateTextures(GL_TEXTURE_2D, 1, &_ID);
    //Set the parameters
    glTextureParameteri(_ID, GL_TEXTURE_WRAP_S, _repeat ? GL_REPEAT : GL_CLAMP_TO_BORDER);
    glTextureParameteri(_ID, GL_TEXTURE_WRAP_T, _repeat ? GL_REPEAT : GL_CLAMP_TO_BORDER);
    int interpolation = _smooth ? GL_LINEAR : GL_NEAREST;
    glTextureParameteri(_ID, GL_TEXTURE_MAG_FILTER, interpolation);
    glTextureParameteri(_ID, GL_TEXTURE_MIN_FILTER, _mipmaped ? GL_LINEAR_MIPMAP_LINEAR : interpolation);
    //Allocate the VRAM on GPU
    unsigned int level = _mipmaped ? std::max(Utilities::log2(_width), Utilities::log2(_height)) : 1;
    glTextureStorage2D(_ID, level, GL_RGBA8, _width, _height);
    //Generate the handle and make the texture resident
    _handle = glGetTextureHandleARB(_ID);
    glMakeTextureHandleResidentARB(_handle);
}

void Image::deallocate()
{
    if (allocated())
    {
        //Delete texture
        if (glIsTextureHandleResidentARB(_handle))
        {
            glMakeTextureHandleNonResidentARB(_handle);
        }
        glDeleteTextures(1, &_ID);
        _ID = 0;
        _handle = 0;
        //Delete FBOs
        delete _FBOs;
        _FBOs = nullptr;
    }
}

bool Image::allocated() const
{
    return (_ID != 0);
}

Image& Image::operator=(const Image& other)
{
    //Change dimensions
    unsigned int width_previous = _width;
    unsigned int height_previous = _height;
    _width = other.width();
    _height = other.height();
    //Copy GPU side data
    if (other.allocated())
    {
        if (width_previous != other.width() || height_previous != other.height())
        {
            deallocate();
            allocate();
        }
        glCopyImageSubData(other.ID(), GL_TEXTURE_2D, 0, 0, 0, 0,
                           _ID, GL_TEXTURE_2D, 0, 0, 0, 0,
                           _width, _height, 1);
    }
    else
    {
        deallocate();
    }
    //Copy CPU side data
    pixels = other.pixels;
    //return reference
    return *this;
}

unsigned int Image::ID() const
{
    return _ID;
}

uint64_t Image::handle() const
{
    return _handle;
}

unsigned int Image::FBO() const
{
    OpenGLContext context = Engine::active_context();
    std::list<std::pair<OpenGLContext, unsigned int>>::iterator it = _FBOs->begin();
    while (it != _FBOs->end())
    {
        if (it->first == context)
        {
            return it->second;
        }
        if (std::find(Engine::contexts.begin(), Engine::contexts.end(), it->first) == Engine::contexts.end())
        {
            it = _FBOs->erase(it);
        }
        else
        {
            it++;
        }
    }
    unsigned int new_FBO = generate_FBO();
    _FBOs->push_back(std::make_pair(context, new_FBO));
    return new_FBO;
}

void Image::update() const
{
    if (_up_to_date)
    {
        return;
    }
    glGenerateTextureMipmap(_ID);
    _up_to_date = true;
}

unsigned int Image::generate_FBO() const
{
    unsigned int fbo = 0;
    glCreateFramebuffers(1, &fbo);
    glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, _ID, 0);
    if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        THROW_ERROR("AntiAliasedSurface of the Image is incomplete after creation.")
    }
    return fbo;
}
