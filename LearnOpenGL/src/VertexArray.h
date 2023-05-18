#pragma once

#include "defines.h"

#include <glad/glad.h>

class VertextArray
{
public:
    VertextArray();
    ~VertextArray()
    {
    }

    void Bind();
    void Unbind();
    void Destroy();

    void LinkAttrib(u32 layout, u32 nComponents, u32 type, i32 stride, void* offset);

private:
    u32 id;
};
