#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, u32 size, u32 mode)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, mode);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Destroy()
{
    glDeleteBuffers(1, &id);
}
