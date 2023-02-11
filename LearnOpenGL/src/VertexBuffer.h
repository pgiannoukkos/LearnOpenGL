#pragma once

#include "glad/glad.h"

class VertexBuffer
{
public:
    VertexBuffer() : id(0) {}
    VertexBuffer(const void* data, size_t size, GLenum mode);
    ~VertexBuffer() {}

    void Bind();
    void Unbind();
    void Destroy();

private:
    unsigned int id;
};

