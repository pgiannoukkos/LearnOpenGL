#pragma once
#include "glad/glad.h"

class IndexBuffer
{
public:
    IndexBuffer() : id(0) {}
    IndexBuffer(const void* data, size_t size, GLenum mode);
    ~IndexBuffer() {}

    void Bind();
    void Unbind();
    void Destroy();

private:
    unsigned int id;
};

