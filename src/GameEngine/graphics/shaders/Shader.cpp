#include <GameEngine/graphics/shaders/Shader.hpp>
#include <GameEngine/Engine.hpp>
using namespace GameEngine;

Shader::Shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    Engine::initialize();
    //load shader code
    _vertex_shader_code = vertex_shader;
    _fragment_shader_code = fragment_shader;
    //Creating shader programs
    compile_link_program();
}

Shader::Shader(const std::string& vertex_shader, const std::string& geometry_shader, const std::string& fragment_shader)
{
    Engine::initialize();
    //load shader code
    _vertex_shader_code = vertex_shader;
    _fragment_shader_code = fragment_shader;
    _geometry_shader_code = geometry_shader;
    //Creating shader programs
    compile_link_program();
}

Shader::~Shader()
{
    //deleting program
    glDeleteProgram(_program_ID);
}

GLuint Shader::ID() const
{
    return _program_ID;
}

void Shader::load_text_file(const std::string& shader_code_path, std::string& destination)
{
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shader_file.open(shader_code_path);
        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        destination = shader_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        THROW_ERROR(std::string("Failed to load the shader's code from file '")+shader_code_path+std::string("' : ")+strerror(errno))
    }
}

void Shader::compile_link_program()
{
    GLuint vertex_shader_ID = compile_shader_code(_vertex_shader_code, GL_VERTEX_SHADER);
    GLuint fragment_shader_ID = compile_shader_code(_fragment_shader_code, GL_FRAGMENT_SHADER);
    GLuint geometry_shader_ID = compile_shader_code(_geometry_shader_code, GL_GEOMETRY_SHADER);
    link_shader_program(vertex_shader_ID, fragment_shader_ID, geometry_shader_ID);
}

GLuint Shader::compile_shader_code(const std::string& shader_code, GLenum shader_type)
{
    if (not(shader_code.empty()))
    {
        //Compiling the shader's code
        GLuint shader_ID = glCreateShader(shader_type);
        const char* shader_code_str = shader_code.c_str();
        glShaderSource(shader_ID, 1, &shader_code_str, NULL);
        glCompileShader(shader_ID);
        //Testing if the shader's compilation went well
        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader_ID, 1024, NULL, infoLog);
            std::string shader_type_name = "'Unknown shader type'";
            if (shader_type == GL_VERTEX_SHADER)
            {
                shader_type_name = "vertex";
            }
            else if (shader_type == GL_FRAGMENT_SHADER)
            {
                shader_type_name = "fragment";
            }
            else if (shader_type == GL_GEOMETRY_SHADER)
            {
                shader_type_name = "geometry";
            }
            THROW_ERROR(std::string("Failed to compile the "+shader_type_name+" shader :\n")+std::string(infoLog)+"\n"+shader_code)
        }
        //Returning the ID of the compiled shader
        return shader_ID;
    }
    else
    {
        return 0;
    }
}

void Shader::link_shader_program(GLuint vertex_shader_ID, GLuint fragment_shader_ID, GLuint geometry_shader_ID)
{
    if (_program_ID != 0)
    {
        glDeleteProgram(_program_ID);
    }
    _program_ID = glCreateProgram();
    if (vertex_shader_ID != 0)
    {
        glAttachShader(_program_ID, vertex_shader_ID);
    }
    if (fragment_shader_ID != 0)
    {
        glAttachShader(_program_ID, fragment_shader_ID);
    }
    if(geometry_shader_ID != 0)
    {
        glAttachShader(_program_ID, geometry_shader_ID);
    }
    glLinkProgram(_program_ID);
    //delete the shaders
    delete_shaders(vertex_shader_ID, fragment_shader_ID, geometry_shader_ID);
    //Test the success of the linking
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(_program_ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(_program_ID, 1024, NULL, infoLog);
        std::cerr << infoLog << std::endl;
        THROW_ERROR("Failed to link the shader program")
    }
}

void Shader::delete_shaders(GLuint vertex_shader_ID, GLuint fragment_shader_ID, GLuint geometry_shader_ID)
{
    if (vertex_shader_ID != 0)
    {
        glDeleteShader(vertex_shader_ID);
    }
    if (fragment_shader_ID != 0)
    {
        glDeleteShader(fragment_shader_ID);
    }
    if (geometry_shader_ID != 0)
    {
        glDeleteShader(geometry_shader_ID);
    }
}
