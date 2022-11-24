#pragma once
#include "glad/glad.h"

class IndexBuffer
{
public:
    IndexBuffer(const void* data, unsigned int size, GLenum mode);
    ~IndexBuffer() {}

    void Bind();
    void Unbind();
    void Destroy();

private:
    unsigned int id;
};

