#include <GameEngine/interface/Window.hpp>
#include <GameEngine/interface/Keyboard.hpp>
#include <GameEngine/interface/Mouse.hpp>
#include <GameEngine/world/Model/Model.hpp>
#include <GameEngine/graphics/shaders/shaders.hpp>
#include <GameEngine/Engine.hpp>
using namespace GameEngine;

WindowSettings Window::settings;

Window::Window() : Image(Window::settings.width, Window::settings.height), keyboard(this), mouse(this)
{
    Engine::initialize();
    create_window(Window::settings);
}

Window::Window(const std::string& title, unsigned int width, unsigned int height) : Image(width, height), keyboard(this), mouse(this)
{
    Engine::initialize();
    WindowSettings settings;
    settings.title = title;
    settings.width = width;
    settings.height = height;
    create_window(settings);
}

Window::Window(const WindowSettings& settings) : Image(settings.width, settings.height), keyboard(this), mouse(this)
{
    Engine::initialize();
    create_window(settings);
}

Window::~Window()
{
    if (Engine::active_context() == this->context())
    {
        Engine::set_active_context(Engine::default_context());
    }
    Engine::contexts.remove(_glfw_window);
    glfwDestroyWindow(_glfw_window);
}

void Window::update()
{
    Engine::set_active_context(context());
    //Polling events
    keyboard.set_unchanged();
    mouse.set_unchanged();
    glfwPollEvents();
    //Drawing to screen
    glViewport(0, 0, _width, _height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Engine::shader2D->draw(*this);
    //Engine::shader_test->draw();
    glfwSwapBuffers(_glfw_window);
}

void Window::resize(unsigned int width, unsigned int height)
{
    glfwSetWindowSize(_glfw_window, width, height);
}

void Window::close()
{
    glfwSetWindowShouldClose(_glfw_window, true);
}

bool Window::is_open() const
{
    return not(glfwWindowShouldClose(_glfw_window));
}

void Window::title(const std::string& name)
{
    _title = name;
    return glfwSetWindowTitle(_glfw_window, name.c_str());
}

const std::string& Window::title() const
{
    return _title;
}

bool Window::has_focus()
{
    return glfwGetWindowAttrib(_glfw_window, GLFW_FOCUSED);
}

void Window::focus()
{
    glfwFocusWindow(_glfw_window);
}

bool Window::resizable()
{
    return glfwGetWindowAttrib(_glfw_window, GLFW_RESIZABLE);
}

void Window::resizable(bool resizable)
{
    glfwSetWindowAttrib(_glfw_window, GLFW_RESIZABLE, resizable);
}

bool Window::borders()
{
    return glfwGetWindowAttrib(_glfw_window, GLFW_DECORATED);
}

void Window::borders(bool borders)
{
    glfwSetWindowAttrib(_glfw_window, GLFW_DECORATED, borders);
}

bool Window::transparent()
{
    return glfwGetWindowAttrib(_glfw_window, GLFW_TRANSPARENT_FRAMEBUFFER);
}

void Window::transparent(bool transparent)
{
    glfwSetWindowAttrib(_glfw_window, GLFW_TRANSPARENT_FRAMEBUFFER, transparent);
}

bool Window::vsync() const
{
    return _vsync;
}

void Window::vsync(bool enabled)
{
    _vsync = enabled;
    if (enabled)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }
}

OpenGLContext Window::context() const
{
    return _glfw_window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
    Window* h = static_cast<Window*>(glfwGetWindowUserPointer(window));
    reinterpret_cast<Image*>(h)->resize(width, height);
}

void Window::create_window(const WindowSettings& settings)
{
    //debug, todo : remove me
    Utilities::check_GL_error();
    //Create the GLFW window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, settings.transparent);
    glfwWindowHint(GLFW_DECORATED, settings.borders);
    unsigned int width = settings.width;
    unsigned int height = settings.height;
    GLFWmonitor* monitor = nullptr;
    const GLFWvidmode* mode = nullptr;
    if (settings.full_screen)
    {
        monitor = glfwGetPrimaryMonitor();
        mode = glfwGetVideoMode(monitor);
        width = mode->width;
        height = mode->height;
        Image::resize(width, height);
    }
    _vsync = settings.vsync;
    _title = settings.title;
    _glfw_window = glfwCreateWindow(width, height, _title.c_str(), monitor, Engine::default_context());
    if (_glfw_window == nullptr)
    {
        THROW_ERROR("Failed to create the window")
    }
    Engine::contexts.push_back(_glfw_window);
    //Link to the keyboard and mouse events
    glfwSetWindowUserPointer(_glfw_window, this);
    mouse.link();
    keyboard.link();
    //set the resize callback
    glfwSetWindowSizeCallback(_glfw_window, resize_callback);
    //Set the window as the current OpenGL context and set some OpenGL flags
    Engine::set_active_context(this);
    glfwSwapInterval(_vsync ? 1 : 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_ONE, GL_ZERO);
    glViewport(0, 0, settings.width, settings.height);
}
