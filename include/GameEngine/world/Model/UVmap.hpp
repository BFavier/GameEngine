#pragma once
#include <GameEngine/Utilities.hpp>
#include "UV.hpp"

 namespace GameEngine
 {
    class Mesh;

    class UVmap
    {
    public:
        UVmap();
        UVmap(const UVmap& other);
        UVmap(const std::initializer_list<UV>& UVs);
        UVmap(const std::vector<UV>& UVs);
        ~UVmap();
    public:
        ///< Returns the number of UVs stored on the GPU side
        unsigned int size() const;
        void upload();
        void allocate();
        void deallocate();
        bool allocated() const;
        UVmap& operator=(const UVmap& other);
        unsigned int ID() const;
        uint64_t handle() const;
    public:
        std::vector<UV> UVs;
        unsigned int n_users = 1;
    protected:
        unsigned int _ID = 0;
        uint64_t _handle = 0;
        unsigned int _size = 0;
    };
 }
