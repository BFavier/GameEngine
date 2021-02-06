#pragma once
#include "Utilities.hpp"

namespace GameEngine
{
    class Window;
    class Shader3D;
    class Shader2D;
    class ShaderTest;//debug, TODO: remove me

    class Engine
    {
    public:
        ///< Initialize the GameEngine
        static void initialize();
        ///< Return the currently active OpenGL context
        static OpenGLContext active_context();
        ///< Set the given OpenGL context as active
        static void set_active_context(OpenGLContext context);
        ///< Set as active the OpenGL linked to the given Window
        static void set_active_context(const Window* window);
        ///< Returns the default OpenGL context, created before any GameEngine::Window object is created
        static OpenGLContext default_context();
    public:
        static Shader3D* shader3D;
        static Shader2D* shader2D;
        //static ShaderTest* shader_test;//debug, TODO: remove me
        static std::list<OpenGLContext> contexts;
    protected:
        ///< If true, the game engine was already initialized
        static bool _initialized;
        ///< An hidden GLFW window created to obtain an Opengl context. This way models can be loaded before the creation of any window.
        static OpenGLContext _default_context;
        ///< The currently active OpenGL context (one openGL context per window)
        static OpenGLContext _active_context;
    };
}
