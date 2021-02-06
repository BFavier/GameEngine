#include <GameEngine/interface/Mouse.hpp>
#include <GameEngine/interface/Window.hpp>
using namespace GameEngine;

Mouse::Mouse(Window* window)
{
    _window = window;
    _buttons["LEFT CLICK"];
    _buttons["RIGHT CLICK"];
    _buttons["MIDDLE CLICK"];
    _buttons["MB4"];
    _buttons["MB5"];
    _buttons["MB6"];
    _buttons["MB7"];
    _buttons["MB8"];
}

Mouse::~Mouse()
{
    _buttons.clear();
}

Button& Mouse::button(const std::string& button_name)
{
    if (_buttons.find(button_name) == _buttons.end())
    {
        THROW_ERROR("The button " + button_name + " doesn't exist.")
    }
    return _buttons.at(button_name);
}

const std::map<std::string, Button>& Mouse::buttons() const
{
    return _buttons;
}

double Mouse::x()
{
    return _x;
}

double Mouse::y()
{
    return _y;
}

double Mouse::dx()
{
    return _dx;
}

double Mouse::dy()
{
    return _dy;
}

double Mouse::wheel_x()
{
    return _wheel_x;
}

double Mouse::wheel_y()
{
    return _wheel_y;
}

bool Mouse::hidden() const
{
    return _hidden;
}

void Mouse::hide(bool h)
{
    if (h)
    {
        glfwSetInputMode(_window->context(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(_window->context(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    _hidden = h;
}

void Mouse::button_callback(GLFWwindow* window, int button, int action, int mods)
{
    (void)mods;//Silence the annoying unused parameter warning
    Window* h = static_cast<Window*>(glfwGetWindowUserPointer(window));
    std::string name;
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        name = "LEFT CLICK";
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        name = "RIGHT CLICK";
    }
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        name = "MIDDLE CLICK";
    }
    else
    {
        name = "MB"+std::to_string(button+1);
    }
    Button& status = h->mouse.button(name);
    if (action == GLFW_PRESS && status.down == false)
    {
        status.down = true;
        status.changed = true;
    }
    else if (action == GLFW_RELEASE && status.down == true)
    {
        status.down = false;
        status.changed = true;
    }
}

void Mouse::position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Window* h = static_cast<Window*>(glfwGetWindowUserPointer(window));
    Mouse& mouse = h->mouse;
    mouse.new_position(xpos, ypos);
}

void Mouse::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* h = static_cast<Window*>(glfwGetWindowUserPointer(window));
    Mouse& mouse = h->mouse;
    mouse.new_wheel(xoffset, yoffset);
}

void Mouse::set_unchanged()
{
    for (auto& [name, status] : _buttons)
    {
        status.changed = false;
    }
    _dx = 0.;
    _dy = 0.;
    _wheel_x = 0.;
    _wheel_y = 0.;
}

void Mouse::new_button(const std::string& name, Button status)
{
    _buttons[name] = status;
}

void Mouse::new_position(double x, double y)
{
    _dx += x - _x;
    _dy += y - _y;
    _x = x;
    _y = y;
}

void Mouse::new_wheel(double wheel_x, double wheel_y)
{
    _wheel_x += wheel_x;
    _wheel_y += wheel_y;
}

void Mouse::link()
{
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(_window->context(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    glfwSetMouseButtonCallback(_window->context(), button_callback);
    glfwSetCursorPosCallback(_window->context(), position_callback);
    glfwSetScrollCallback(_window->context(), scroll_callback);
    glfwGetCursorPos(_window->context(), &_x, &_y);
}
