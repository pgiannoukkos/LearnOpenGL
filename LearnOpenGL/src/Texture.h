#pragma once

#include <glad/glad.h>

#include <string>

class Texture
{
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind();

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

private:
    unsigned int m_TextureID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BBP;
    GLenum m_Format;
};
