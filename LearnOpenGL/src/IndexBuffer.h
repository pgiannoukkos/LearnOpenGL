#pragma once

#include "defines.h"

#include <glad/glad.h>

class IndexBuffer
{
public:
    IndexBuffer() : id(0)
    {
    }

    IndexBuffer(const void* data, u32 size, u32 mode);

    ~IndexBuffer()
    {
    }

    void Bind();
    void Unbind();
    void Destroy();

private:
    unsigned int id;
};
