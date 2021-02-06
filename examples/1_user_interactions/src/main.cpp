#include <GameEngine/GameEngine.hpp>
using namespace GameEngine;

int main(int argc, char* argv[])
{
    Window window;
    Keyboard& keyboard = window.keyboard;
    Mouse& mouse = window.mouse;
    while (window.is_open())
    {
        if (mouse.dx() != 0. || mouse.dy() != 0.)
        {
            std::cout << "x = " << mouse.x() << " y = " << mouse.y() << std::endl;
        }
        if (mouse.wheel_x() != 0.)
        {
            std::cout << "wheel x = " << mouse.wheel_x() << std::endl;
        }
        if (mouse.wheel_y() != 0.)
        {
            std::cout << "wheel y = " << mouse.wheel_y() << std::endl;
        }
        for (const auto& [key, status] : keyboard.keys())
        {
            if (status.changed && status.down)
            {
                std::cout << key << " was pressed" << std::endl;
            }
        }
        for (const auto& [button, status] : mouse.buttons())
        {
            if (status.changed && status.down)
            {
                std::cout << button << " was pressed" << std::endl;
            }
        }
        if (keyboard.key("ESCAPE").down)
        {
            window.close();
        }
        if (keyboard.key("ENTER").down && keyboard.key("ENTER").changed)
        {
            mouse.hide(!mouse.hidden());
        }
        window.update();
    }
	return 0;
}
