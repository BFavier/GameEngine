#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/graphics/RGB.hpp>

namespace GameEngine
{
    class RGBA : public RGB
    {
        public:
            RGBA();
            RGBA(unsigned char R, unsigned char G, unsigned char B, unsigned char A);
            RGBA(const RGBA& other);
            RGBA(const std::initializer_list<unsigned char>& init);
            ~RGBA();
        public:
            RGBA operator+(const RGBA& other) const;
            RGBA& operator+=(const RGBA& other);
            RGBA& operator=(const RGBA& other);
            friend std::ostream& operator<<(std::ostream& os, const RGBA& rgba);
        public:
            unsigned char A = 255;
    };
}
