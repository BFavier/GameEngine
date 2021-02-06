#pragma once
#include <GameEngine/Utilities.hpp>
#include "Button.hpp"

namespace GameEngine
{
    class Window;

    class Keyboard
    {
    public:
        Keyboard() = delete;
        Keyboard(Window* window);
        ~Keyboard();
        const std::map<std::string, Button>& keys() const;
        Button& key(const std::string& name);
    public:
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void set_unchanged();
        void link();
    protected:
        static std::string get_key_name(int key, int scancode);
    private:
        Window* _window = nullptr;
        std::map<std::string, Button> _keys;
    };
}
