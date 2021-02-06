#include <GameEngine/graphics/Image/Gbuffer.hpp>
using namespace GameEngine;

Gbuffer::Gbuffer()
{
    Engine::initialize();
    allocate();
}

Gbuffer::Gbuffer(unsigned int width, unsigned int height, unsigned int AA, unsigned int transparency_layers)
{
    Engine::initialize();
    _width = width;
    _height = height;
    _AA = AA;
    _transparency_layers = transparency_layers;
    allocate();
}

Gbuffer::~Gbuffer()
{
    deallocate();
}

void Gbuffer::render(const Model& model, const Camera& camera)
{
    unsigned int fbo = FBO();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    Engine::shader3D->draw(model, camera, width(), height(), _layers);
    //glMemoryBarrier(GL_FRAMEBUFFER_BARRIER_BIT);
}

void Gbuffer::clear(const RGBA& color)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO());
    glClearColor(color.R, color.G, color.B, color.A);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    float float_fill[3] = {0.};
    glClearTexImage(_position_ID, 0, GL_RGB, GL_FLOAT, &float_fill);
    glClearTexImage(_normal_ID, 0, GL_RGB, GL_FLOAT, &float_fill);
    glClearTexImage(_material_ID, 0, GL_RGB, GL_FLOAT, &float_fill);
}

void Gbuffer::resize(unsigned int width, unsigned int height)
{
    if (_width != width || _height != height)
    {
        deallocate();
        _width = width;
        _height = height;
        allocate();
    }
}

unsigned int Gbuffer::width() const
{
    return _width;
}

unsigned int Gbuffer::height() const
{
    return _height;
}

unsigned int Gbuffer::AA() const
{
    return _AA;
}

void Gbuffer::AA(unsigned int samples)
{
    if (_AA != samples)
    {
        deallocate();
        _AA = samples;
        allocate();
    }
}

unsigned int Gbuffer::transparency_layers() const
{
    return _transparency_layers;
}

void Gbuffer::transparency_layers(unsigned int number) const
{
    if (_transparency_layers != number)
    {
        deallocate();
        _transparency_layers = number;
        allocate();
    }
}

Gbuffer& Gbuffer::operator=(const Gbuffer& other)
{
    resize(other.width(), other.height());
    AA(other.AA());
    return *this;
}

unsigned int Gbuffer::color_ID() const
{
    return _color_ID;
}

unsigned int Gbuffer::position_ID() const
{
    return _position_ID;
}

unsigned int Gbuffer::normal_ID() const
{
    return _normal_ID;
}

unsigned int Gbuffer::pointer_ID() const
{
    return _pointer_ID;
}

unsigned int Gbuffer::layers_ID() const
{
    return _layers_ID;
}

uint64_t Gbuffer::color_handle() const
{
    return _color_handle;
}

uint64_t Gbuffer::position_handle() const
{
    return _position_handle;
}

uint64_t Gbuffer::normal_handle() const
{
    return _normal_handle;
}

uint64_t Gbuffer::layers_handle() const
{
    return _layers_handle;
}

unsigned int Gbuffer::FBO()
{
    OpenGLContext context = Engine::active_context();
    std::list<std::pair<OpenGLContext, unsigned int>>::iterator it = _FBOs.begin();
    while (it != _FBOs.end())
    {
        if (it->first == context)
        {
            return it->second;
        }
        if (std::find(Engine::contexts.begin(), Engine::contexts.end(), it->first) == Engine::contexts.end())
        {
            it = _FBOs.erase(it);
        }
        else
        {
            it++;
        }
    }
    unsigned int new_FBO = generate_FBO();
    _FBOs.push_back(std::make_pair(context, new_FBO));
    return new_FBO;
}

void Gbuffer::allocate()
{
    if (width > 0 && height > 0)
    {
        //Generate color texture
        glCreateTextures(1, &_color_ID);
        glTextureStorage2DMultisample(_color_ID, _AA, GL_RGBA32F, _width, _height);
        _color_handle = glGetImageHandleARB(_color_ID, 0, true, 0, GL_RGBA32F);
        glMakeImageHandleResidentARB(_color_handle, GL_READ);
        //Generate position texture
        glCreateTextures(1, &_position_ID);
        glTextureStorage2DMultisample(_position_ID, _AA, GL_RGB32F, _width, _height);
        _position_handle = glGetImageHandleARB(_position_ID, 0, true, 0, GL_RGB32F);
        glMakeImageHandleResidentARB(_position_handle, GL_READ);
        //Generate normal texture
        glCreateTextures(1, &_normal_ID);
        glTextureStorage2DMultisample(_normal_ID, _AA, GL_RGB32F, _width, _height);
        _normal_handle = glGetImageHandleARB(_normal_ID, 0, true, 0, GL_RGB32F);
        glMakeImageHandleResidentARB(_normal_handle, GL_READ);
        //Generate material texture
        glCreateTextures(1, &_material_ID);
        glTextureStorage2DMultisample(_material_ID, _AA, GL_RGB32F, _width, _height);
        _material_handle = glGetImageHandleARB(_material_ID, 0, true, 0, GL_RGB32F);
        glMakeImageHandleResidentARB(_material_handle, GL_READ);
        //Generate pointer texture
        glCreateTextures(1, &_pointer_ID);
        glTextureStorage2D(_pointer_ID, 1, GL_RG32F, _width, _height);
        //Generate data layers
        unsigned int depth = _transparency_layers*15;
        if (depth > 0)
        {
            glCreateTextures(GL_TEXTURE_3D, 1, &data_layers);
            glTextureStorage3D(data_layers, 1, GL_R32F, _width, _height, depth);
            _layers_handle = glGetImageHandleARB(_layers_ID, 0, true, 0, GL_R32F);
            glMakeImageHandleResidentARB(_layers_handle, GL_READ_WRITE);
        }
        //Generate depth buffer
        glCreateRenderbuffers(1, &_depth_buffer);
        glNamedRenderbufferStorageMultisample(_depth_buffer, _AA, GL_DEPTH_COMPONENT32F, _width, _height);
        //Generate stencil buffer
        glCreateRenderbuffers(1, &_stencil_buffer);
        glNamedRenderbufferStorageMultisample(_stencil_buffer, _AA, GL_STENCIL_INDEX8, _width, _height);
    }
    //Initialize buffers
    clear({0., 0., 0., 0.});
}

void Gbuffer::deallocate()
{
    //Delete FBOs
    for (std::pair<OpenGLContext, unsigned int>& elements : _FBOs)
    {
        glDeleteFramebuffers(1, &elements.second);
    }
    _FBOs.clear();
    //Delete color texture
    if (glIsImageHandleResidentARB(_color_handle))
    {
        glMakeImageHandleNonResidentARB(_color_handle);
    }
    glDeleteTextures(1, &_color_ID);
    //Delete position texture
    if (glIsImageHandleResidentARB(_position_handle))
    {
        glMakeImageHandleNonResidentARB(_position_handle);
    }
    glDeleteTextures(1, &_position_ID);
    //Delete normal texture
    if (glIsImageHandleResidentARB(_normal_handle))
    {
        glMakeImageHandleNonResidentARB(_normal_handle);
    }
    glDeleteTextures(1, &_normal_ID);
    //Delete material texture
    if (glIsImageHandleResidentARB(_material_handle))
    {
        glMakeImageHandleNonResidentARB(_material_handle);
    }
    glDeleteTextures(1, &_material_ID);
    //Delete pointer texture
    glDeleteTextures(1, &_pointer_ID);
    //Delete layers texture
    if (glIsImageHandleResidentARB(_layers_handle))
    {
        glMakeImageHandleNonResidentARB(_layers_handle);
    }
    glDeleteTextures(1, &_layers_ID);
    //Delete stencil buffer
    glDeleteRenderbuffers(1, &_depth_buffer);
    //Delete stencil buffer
    glDeleteRenderbuffers(1, &_stencil_buffer);
}

unsigned int Gbuffer::generate_FBO() const
{
    unsigned int fbo;
    glCreateFramebuffers(1, &fbo);
    //Set color texture
    glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, _color_texture, 0);
    //Set position texture
    glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT1, _position_texture, 0);
    //Set normal texture
    glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT2, _normal_texture, 0);
    //Set material texture
    glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT3, _material_texture, 0);
    //Set pointer texture
    glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT4, _pointer_texture, 0);
    //Set depth buffer
    glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, _depth_buffer);
    //Set stencil buffer
    glNamedFramebufferRenderbuffer(fbo, GL_STENCIL_ATTACHMENT,  GL_RENDERBUFFER, _stencil_buffer);
    //Setting the outputs order
    unsigned int attachements[5] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
    glNamedFramebufferDrawBuffers(fbo, 5, attachements);
    //Checking completeness of the frame buffer object
    if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        THROW_ERROR("FBO of the Gbuffer is incomplete after creation.")
    }
    //returning FBO
    return fbo;
}
