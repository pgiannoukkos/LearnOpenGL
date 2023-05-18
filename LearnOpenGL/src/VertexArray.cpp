#include "VertexArray.h"

VertextArray::VertextArray()
{
    glGenVertexArrays(1, &id);
}

void VertextArray::Bind()
{
    glBindVertexArray(id);
}

void VertextArray::Unbind()
{
    glBindVertexArray(0);
}

void VertextArray::Destroy()
{
    glDeleteVertexArrays(1, &id);
}

void VertextArray::LinkAttrib(u32 layout, u32 nComponents, u32 type, i32 stride, void* offset)
{
    glBindVertexArray(id);
    glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, nComponents, type, GL_FALSE, stride, offset);
    glBindVertexArray(0);
}
