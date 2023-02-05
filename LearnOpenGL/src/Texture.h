#pragma once

#include <glad/glad.h>

#include <string>

class Texture
{
public:
    Texture(const std::string& path, const std::string& type);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind();

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

    inline std::string GetType() const { return m_Type; }

private:
    unsigned int m_TextureID;
    unsigned char* m_LocalBuffer;
    std::string m_FilePath;
    std::string m_Type;
    int m_Width, m_Height, m_BBP;
    GLenum m_Format;
};
