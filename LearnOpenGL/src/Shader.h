#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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

    void Destroy();
};

