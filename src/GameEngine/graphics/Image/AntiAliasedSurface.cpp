#include <GameEngine/graphics/Image/AntiAliasedSurface.hpp>
using namespace GameEngine;

AntiAliasedSurface::AntiAliasedSurface()
{
    Engine::initialize();
    allocate();
}

AntiAliasedSurface::AntiAliasedSurface(unsigned int width, unsigned int height, unsigned int AA)
{
    Engine::initialize();
    _width = width;
    _height = height;
    _AA = AA;
    allocate();
}

AntiAliasedSurface::~AntiAliasedSurface()
{
    deallocate();
}

void AntiAliasedSurface::clear(const RGBA& color)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO());
    glClearColor(color.R, color.G, color.B, color.A);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

unsigned int AntiAliasedSurface::FBO()
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

void AntiAliasedSurface::resize(unsigned int width, unsigned int height)
{
    if (_width != width || _height != height)
    {
        deallocate();
        _width = width;
        _height = height;
        allocate();
    }
}

unsigned int AntiAliasedSurface::width() const
{
    return _width;
}

unsigned int AntiAliasedSurface::height() const
{
    return _height;
}

unsigned int AntiAliasedSurface::AA() const
{
    return _AA;
}

void AntiAliasedSurface::AA(unsigned int samples)
{
    if (_AA != samples)
    {
        deallocate();
        _AA = samples;
        allocate();
    }
}

AntiAliasedSurface& AntiAliasedSurface::operator=(const AntiAliasedSurface& other)
{
    resize(other.width(), other.height());
    AA(other.AA());
    return *this;
}

void AntiAliasedSurface::allocate()
{
    //Generate color buffer
    glCreateRenderbuffers(1, &_color_buffer);
    glNamedRenderbufferStorageMultisample(_color_buffer, _AA, GL_RGBA32F, _width, _height);
    //Generate depth buffer
    glCreateRenderbuffers(1, &_depth_buffer);
    glNamedRenderbufferStorageMultisample(_depth_buffer, _AA, GL_DEPTH24_STENCIL8, _width, _height);
    //Initialize buffers
    clear({0., 0., 0., 0.});
}

void AntiAliasedSurface::deallocate()
{
    //Delete FBOs
    for (std::pair<OpenGLContext, unsigned int>& elements : _FBOs)
    {
        glDeleteFramebuffers(1, &elements.second);
    }
    _FBOs.clear();
    //Delete color buffer
    glDeleteRenderbuffers(1, &_color_buffer);
    //Delete depth buffer
    glDeleteRenderbuffers(1, &_depth_buffer);
}

unsigned int AntiAliasedSurface::generate_FBO() const
{
    unsigned int fbo;
    glCreateFramebuffers(1, &fbo);
    //Set color buffer
    glNamedFramebufferRenderbuffer(fbo, GL_COLOR_ATTACHMENT0,  GL_RENDERBUFFER, _color_buffer);
    //Set the depth buffer
    glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_STENCIL_ATTACHMENT,  GL_RENDERBUFFER, _depth_buffer);
    //Checking completeness of the frame buffer object
    if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        THROW_ERROR("FBO of the AntiAliasedSurface is incomplete after creation.")
    }
    return fbo;
}
