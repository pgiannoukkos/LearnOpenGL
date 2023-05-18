#include "Shader.h"

#include "Log.h"

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
    // 1. retrive the vertex/fragment source code from file path
    std::string vertex_src;
    std::string fragment_src;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    // ensure ifstream objects can throw exceptrions:
    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        v_shader_file.open(vertex_path);
        f_shader_file.open(fragment_path);
        std::stringstream v_shader_stream, f_shader_stream;

        // read file's buffer contents into streams
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();

        // close the file handlers
        v_shader_file.close();
        f_shader_file.close();

        // convert stream into string
        vertex_src = v_shader_stream.str();
        fragment_src = f_shader_stream.str();
    }
    catch (const std::ifstream::failure e) {
        LOG_ERROR("SHADER: File not succesfully read.");
    }

    const char* v_shader_src = vertex_src.c_str();
    const char* f_shader_src = fragment_src.c_str();

    // 2. compile shaders
    u32 vertex, fragment;
    i32 success;
    char info_log[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_shader_src, NULL);
    glCompileShader(vertex);

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        LOG_ERROR("Shader: Vertex Shader Compilation Failed.");
        // std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_shader_src, NULL);
    glCompileShader(fragment);

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        LOG_ERROR("Shader: Fragment Shader Compilation Failed.");
        // std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    // check for shader linking errors
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, info_log);
        LOG_ERROR("Shader: Program Linkinkg Failed.");
        // std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use()
{
    glUseProgram(id);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, f32 value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3 v) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z);
}

void Shader::SetVec3(const std::string& name, f32 v0, f32 v1, f32 v2) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), v0, v1, v2);
}

void Shader::SetMat4(const std::string& name, glm::mat4 matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::Destroy()
{
    glDeleteProgram(id);
}
