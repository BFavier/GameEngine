#pragma once
#include <GameEngine/Utilities.hpp>
#include "Button.hpp"

namespace GameEngine
{
    class Window;

    class Mouse
    {
    public:
        Mouse() = delete;
        Mouse(Window* window);
        ~Mouse();
        Button& button(const std::string& button_name);
        const std::map<std::string, Button>& buttons() const;
        double x();
        double y();
        double dx();
        double dy();
        double wheel_x();
        double wheel_y();
        double x_rel();
        double y_rel();
        bool hidden() const;
        void hide(bool hide);
    public:
        static void button_callback(GLFWwindow* window, int button, int action, int mods);
        static void position_callback(GLFWwindow* window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        void set_unchanged();
        void new_button(const std::string& name, Button status);
        void new_position(double x, double y);
        void new_wheel(double wheel_x, double wheel_y);
        void link();
    private:
        bool _hidden = false;
        double _x = 0.;
        double _y = 0.;
        double _dx = 0.;
        double _dy = 0.;
        double _wheel_x = 0.;
        double _wheel_y = 0.;
        std::map<std::string, Button> _buttons;
        Window* _window;
    };
}
