#pragma once
#include <GameEngine/Utilities.hpp>

namespace GameEngine
{
    class RGB
    {
        public:
            RGB();
            RGB(unsigned char R, unsigned char G, unsigned char B);
            RGB(const RGB& other);
            RGB(const std::initializer_list<unsigned char>& init);
            ~RGB();
        public:
            RGB& operator=(const RGB& other);
            friend std::ostream& operator<<(std::ostream& os, const RGB& rgb);
        public:
            unsigned char R = 255;
            unsigned char G = 255;
            unsigned char B = 255;
    };
}
