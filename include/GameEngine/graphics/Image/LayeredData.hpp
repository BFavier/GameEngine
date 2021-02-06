#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/Engine.hpp>

namespace GameEngine
{
    /// \brief Layers of data for each pixel. Used during 3D rendering for transparency layers, and to store some per-pixel data.
    ///
    /// Each position and normal is expressed in camera space.
    /// The opaque layer data are stored as follow:
    /// 3 floats: position of the opaque layer
    /// 3 floats: normal of the opaque layer
    /// 2 floats: pointer of the drawn item
    /// Then for each transparency layer:
    /// 4 floats: color of the transparent layer
    /// 3 floats: position of the transparent layer
    /// 3 floats: normal of the transparent layer
    /// 2 floats: pointer of the drawn item
    ///
    class LayeredData
    {
    public:
        LayeredData();
        LayeredData(unsigned int width, unsigned int height, unsigned int depth);
        ~LayeredData();
    public:
        void clear();
        double& at(unsigned int x, unsigned int y, unsigned int z);
        void resize(unsigned int width, unsigned int height, unsigned int depth);
        unsigned int width() const;
        unsigned int height() const;
        unsigned int depth() const;
        void upload() const;
        void download();
        void allocate();
        void deallocate();
        bool allocated() const;
        LayeredData& operator=(const LayeredData& other);
        unsigned int ID() const;
        uint64_t handle() const;
    public:
        std::vector<double> data = {0.};
    protected:
        unsigned int _ID = 0;
        uint64_t _handle = 0;
        unsigned int _width = 1;
        unsigned int _height = 1;
        unsigned int _depth = 1;
    };
}
