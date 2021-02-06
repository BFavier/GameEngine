#include <GameEngine/Engine.hpp>
#include <GameEngine/interface/Window.hpp>
#include <GameEngine/graphics/shaders/shaders.hpp>
using namespace GameEngine;

Shader3D* Engine::shader3D = nullptr;
Shader2D* Engine::shader2D = nullptr;
//ShaderTest* Engine::shader_test = nullptr;//debug, TODO: remove me
std::list<OpenGLContext> Engine::contexts;
bool Engine::_initialized = false;
OpenGLContext Engine::_default_context = nullptr;
OpenGLContext Engine::_active_context = nullptr;

void Engine::initialize()
{
    if (_initialized)
    {
        return;
    }
    //Initialize GLFW
    if (!glfwInit())
    {
        THROW_ERROR("Failed to initialize the library GLFW")
    }
    //create the default hidden window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    _default_context = glfwCreateWindow(1, 1, "GameEngine", nullptr, nullptr);
    if (_default_context == nullptr)
    {
        THROW_ERROR("During initialization of the GameEngine, failed to create the default OpenGL context")
    }
    Engine::contexts.push_back(_default_context);
    Engine::set_active_context(_default_context);
    //Initialize openGL using glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        THROW_ERROR("Failed to initialize GLAD")
    }
    //Set the initialized flag
    _initialized = true;
    //Create the shader3D
    shader3D = new Shader3D();
    shader2D = new Shader2D();
    //shader_test = new ShaderTest();
}

OpenGLContext Engine::active_context()
{
    return Engine::_active_context;
}

void Engine::set_active_context(OpenGLContext context)
{
    if (context == nullptr)
    {
        THROW_ERROR("'Engine::active_context' received nullptr")
    }
    if (Engine::_active_context == context)
    {
        return;
    }
    Engine::_active_context = context;
    glfwMakeContextCurrent(context);
}

void Engine::set_active_context(const Window* window)
{
    set_active_context(window->context());
}

OpenGLContext Engine::default_context()
{
    return _default_context;
}
