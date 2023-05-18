#pragma once

#include "defines.h"

#include <glad/glad.h>

#include <string>

class Texture
{
public:
    Texture()
    {
    }
    Texture(const std::string& path, const std::string& type);
    ~Texture();

    void Bind(u32 slot = 0) const;
    void Unbind();

    inline i32 GetWidth() const
    {
        return m_Width;
    }
    inline i32 GetHeight() const
    {
        return m_Height;
    }

    inline std::string GetType() const
    {
        return m_Type;
    }

private:
    u32 m_TextureID;
    u8* m_LocalBuffer;
    std::string m_FilePath;
    std::string m_Type;
    i32 m_Width, m_Height, m_BBP;
    GLenum m_Format;
};
