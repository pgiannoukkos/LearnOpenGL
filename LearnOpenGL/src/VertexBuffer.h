#pragma once

#include "defines.h"

#include "glad/glad.h"

class VertexBuffer
{
public:
    VertexBuffer() : id(0)
    {
    }

    VertexBuffer(const void* data, u32 size, u32 mode);

    ~VertexBuffer()
    {
    }

    void Bind();
    void Unbind();
    void Destroy();

private:
    u32 id;
};
