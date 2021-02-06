#pragma once
#include <GameEngine/Utilities.hpp>
#include "Face.hpp"
#include "Vertex.hpp"

namespace GameEngine
{
    class Mesh
    {
        public:
            Mesh();
            Mesh(const Mesh& other);
            Mesh(const std::initializer_list<Face>& elements);
            Mesh(const std::vector<Face>& faces, bool dynamic=false);
            Mesh(const aiMesh* aimesh);
            ~Mesh();
        public:
            ///< Copy the data from CPU to GPU. This must be called after modifications of the mesh vertices/faces for them to take effect. Use with parsimony, CPU/GPU synchronization is relatively slow.
            void upload();
            ///< Return true if the mesh is dynamic. Dynamic mesh's data is stored preferentially on a fast read/write zone of the GPU memory.
            bool dynamic() const;
            ///< Set the dynamic status of the mesh. If the status change, induce a memory copy from CPU to GPU.
            void dynamic(bool enabled);
            ///< Remove all faces and vertices of the mesh
            void clear();
            ///< Deep copy the faces/vertices of the given list of faces. The function 'update_GPU' must be called after for the change to take effect on GPU data.
            void copy_faces(const std::vector<Face>& faces);
            ///< Update the 'index' attribute of each vertex
            void reindex_vertices() const;
            ///< Returns the VAO of the mesh for the current OpenGL context)
            unsigned int VAO();
            ///< Returns the number of vertices currently loaded on GPU
            unsigned int n_vertices_GPU() const;
        public:
            std::vector<Vertex> vertices;
            std::vector<Face> faces;
            unsigned int n_users = 1;
        public:
            Mesh& operator=(const Mesh& other);
        protected:
            ///< whether or not mesh is stored on GPU's fast read/write memory
            bool _dynamic = false;
            ///<
            std::list<std::pair<OpenGLContext, unsigned int>> _VAOs;
            ///< ID of the Vertex Buffer Object (OpenGL)
            unsigned int _VBO = 0;
            ///< ID of the Element buffer object (OpenGL)
            unsigned int _EBO = 0;
            ///< number of vertices stored on GPU
            unsigned int _n_vertices_GPU = 0;
            ///< number of faces stored on GPU
            unsigned int _n_faces_GPU = 0;
        protected:
            ///< copy data from CPU to GPU. Reallocate GPU memory if size of data changed or if 'reallocate_buffers' is true.
            void load_data(bool reallocate_buffers);
            ///< Create the VBO, EBO
            void allocate();
            ///< Delete the VBO, EBO and VAOs
            void deallocate();
            ///< Generate a new VAO
            unsigned int generate_VAO() const;
            ///< Delete all the VAOs;
            void clear_VAOs();
    };
}


