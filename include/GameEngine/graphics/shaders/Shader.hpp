#pragma once
#include <GameEngine/Utilities.hpp>
#include <fstream>
#include <sstream>

namespace GameEngine
{
    class Shader
    {
    public:
        Shader() = delete;
        Shader(const std::string& vertex_shader, const std::string& fragment_shader);
        Shader(const std::string& vertex_shader, const std::string& geometry_shader, const std::string& fragment_shader);
        ~Shader();
        GLuint ID() const;
        static void load_text_file(const std::string& file_path, std::string& destination);
    protected:
        void compile_link_program();
        GLuint compile_shader_code(const std::string& shader_code_path, GLenum shader_type);
        void link_shader_program(GLuint vertex_shader_ID, GLuint fragment_shader_ID, GLuint geometry_shader_ID);
        void delete_shaders(GLuint vertex_shader_ID, GLuint fragment_shader_ID, GLuint geometry_shader_ID);
    protected:
        GLuint _program_ID = 0;
        std::string _vertex_shader_code = "";
        std::string _fragment_shader_code = "";
        std::string _geometry_shader_code = "";
    /*
    protected:
        void set_uniform_var(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(_program_ID, name.c_str()), (int)value);
        }
        void set_uniform_var(const std::string &name, int value) const
        {
            glUniform1i(glGetUniformLocation(_program_ID, name.c_str()), value);
        }
        void set_uniform_var(const std::string &name, unsigned int value) const
        {
            glUniform1ui(glGetUniformLocation(_program_ID, name.c_str()), value);
        }
        void set_uniform_var(const std::string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(_program_ID, name.c_str()), value);
        }
        void set_uniform_var(const std::string &name, const glm::vec2 &value) const
        {
            glUniform2fv(glGetUniformLocation(_program_ID, name.c_str()), 1, &value[0]);
        }
        void set_uniform_var(const std::string &name, float x, float y) const
        {
            glUniform2f(glGetUniformLocation(_program_ID, name.c_str()), x, y);
        }
        void set_uniform_var(const std::string &name, const glm::vec3 &value) const
        {
            glUniform3fv(glGetUniformLocation(_program_ID, name.c_str()), 1, &value[0]);
        }
        void set_uniform_var(const std::string &name, float x, float y, float z) const
        {
            glUniform3f(glGetUniformLocation(_program_ID, name.c_str()), x, y, z);
        }
        void set_uniform_var(const std::string &name, const glm::vec4 &value) const
        {
            glUniform4fv(glGetUniformLocation(_program_ID, name.c_str()), 1, &value[0]);
        }
        void set_uniform_var(const std::string &name, float x, float y, float z, float w)
        {
            glUniform4f(glGetUniformLocation(_program_ID, name.c_str()), x, y, z, w);
        }
        void set_uniform_var(const std::string &name, const glm::mat2 &mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(_program_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void set_uniform_var(const std::string &name, const glm::mat3 &mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(_program_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void set_uniform_var(const std::string &name, const glm::mat4 &mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(_program_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
    */
    };
}
