#pragma once

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    unsigned int id;

    // constructor reads and builds the shader
    Shader(const char* vertex_path, const char* fragment_path);

    // use/activate the shader
    void Use();

    // utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3 v) const;
    void SetVec3(const std::string& name, float v0, float v1, float v2) const;
    void SetMat4(const std::string& name, glm::mat4 matrix) const;

    void Destroy();
};
