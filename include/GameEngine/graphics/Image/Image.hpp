#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/Engine.hpp>
#include <GameEngine/graphics/RGBA.hpp>

namespace GameEngine
{
    class Image
    {
        public:
            Image();
            Image(const Image& other);
            ///< Load an image from disk (supported extensions are: .png, .bmp, .jpg, .tga, .gif, ...)
            Image(const std::string& path);
            ///< Initialize an uniformly colored image
            Image(unsigned int width, unsigned int height, const RGBA& color={0, 0, 0, 0});
            ///< Initialize an image from a list of pixel colors (line by line from top to bottom)
            Image(unsigned int width, unsigned int height, const std::vector<RGBA>& pixels);
            virtual ~Image();
        public:
            ///< Save image on disk as one of the following formats: .png, .jpg, .bmp, .tga
            void save(const std::string& file_path);
            ///< Clear the image's color (on CPU and GPU), as well as the depth buffers used for 3D rendering
            virtual void clear(const RGBA& color);
            ///< Resize the image (on CPU and GPU). The memory on GPU is reallocated and it's content is undefined behavior.
            virtual void resize(unsigned int width, unsigned int height);
            ///< Returns the width of the image
            unsigned int width() const;
            ///< Returns the height of the image
            unsigned int height() const;
            ///< Copy another image (GPU side and CPU side)
            Image& operator=(const Image& other);
        public:
            ///< Access the pixel at position (x, y) on CPU side. Modifications will only be visible on screen if upload() is called after.
            virtual RGBA& pixel(unsigned int x, unsigned int y);
            ///< Copy data stored on CPU side to GPU side
            void upload() const;
            ///< Copy the data stored on GPU side to CPU side
            void download();
            ///< Reallocate the memory on the GPU (it fails if it is already allocated). upload() must then be called to copy the CPU data to GPU.
            void allocate();
            ///< Deallocate the memory on the GPU (do nothing if it was not allocated to begin with). Usefull if the image is not currently used, and you are running low on VRAM.
            void deallocate();
            ///< Returns true if the Image has memory allocated on the GPU
            bool allocated() const;
        public:
            ///< Returns the current ID of the image (called a GL_TEXTURE_2D on OpenGL).
            unsigned int ID() const;
            ///< Returns the current handle of the image (for OpenGL bind-less texture).
            uint64_t handle() const;
            ///< Returns the FBO corresponding to the currently active OpenGL context. Creates one if it doesn't exists. This also delete FBOs of contexts that don't exist anymore.
            unsigned int FBO() const;
            ///< Update the mipmaps of the image on GPU side
            virtual void update() const;
        public:
            ///< Data of the CPU side. This can be cleared to save some CPU memory.
            std::vector<RGBA> pixels;
            ///< If the number of texture using this image falls to 0 when unlinking from a texture, the texture will call the image's destructor.
            bool auto_delete = false;
            ///< Number of textures that are currently using the image.
            unsigned int _n_users = 0;
        protected:
            unsigned int _width = 1;
            unsigned int _height = 1;
            bool _mipmaped = true;
            bool _repeat = false;
            bool _smooth = false;
            unsigned int _ID = 0;
            uint64_t _handle = 0;
            std::list<std::pair<OpenGLContext, unsigned int>>* _FBOs = nullptr;
            bool _up_to_date = true;
        protected:
            unsigned int generate_FBO() const;
    };
}
