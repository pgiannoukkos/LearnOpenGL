#pragma once

#include <glad/glad.h>

class VertextArray
{
public:
    VertextArray();
    ~VertextArray() {}

    void Bind();
    void Unbind();
    void Destroy();

    void LinkAttrib(unsigned int layout, unsigned int nComponents, GLenum type, GLsizeiptr stride, void* offset);

private:
    unsigned int id;
};
