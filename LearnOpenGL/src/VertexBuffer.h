#pragma once

#include "glad/glad.h"

class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size, GLenum mode);
    ~VertexBuffer() {}

    void Bind();
    void Unbind();
    void Destroy();

private:
    unsigned int id;
};

