#include "Texture2D.h"

#include "Log.h"

#include <iostream>
#include <stb_image.h>

Texture2D::Texture2D()
    : m_Width(0), m_Height(0), m_InternalFormat(GL_RGB), m_ImageFormat(GL_RGB), m_WrapS(GL_REPEAT), m_WrapT(GL_REPEAT),
      m_WrapR(GL_REPEAT), m_FilterMin(GL_LINEAR), m_FilterMag(GL_LINEAR), m_DataType(GL_UNSIGNED_BYTE), m_Type(""),
      m_FilePath("")
{
    glGenTextures(1, &m_TextureID);
}

void Texture2D::Generate(u32 width, u32 height, const void* data, b8 mipmap)
{
    m_Width = width;
    m_Height = height;

    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_ImageFormat, m_DataType, data);

    if (mipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
        m_FilterMin = GL_LINEAR_MIPMAP_LINEAR;
    }

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapT);

    // set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMag);

    // unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::GenerateCubemap(u32 width, u32 height, b8 mipmap)
{
    m_Width = width;
    m_Height = height;

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
    for (u32 i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, width, height, 0, m_ImageFormat,
                     m_DataType, nullptr);
    }

    if (mipmap) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
        // m_FilterMin = GL_LINEAR_MIPMAP_LINEAR;
    }

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_WrapS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_WrapT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_WrapR);

    // set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_FilterMin);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_FilterMag);

    // unbind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture2D::Bind(u32 slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture2D::BindCubemap(u32 slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
}

void Texture2D::UnBind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::UnBindCubemap()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture2D::Destroy()
{
    glDeleteTextures(1, &m_TextureID);
}

u32 Texture2D::GetTexID() const
{
    return m_TextureID;
}

u32 Texture2D::GetWidth() const
{
    return m_Width;
}

u32 Texture2D::GetHeight() const
{
    return m_Height;
}

std::string Texture2D::GetType() const
{
    return m_Type;
}

std::string Texture2D::GetPath() const
{
    return m_FilePath;
}

void Texture2D::SetInternalFormat(u32 format)
{
    m_InternalFormat = format;
}

void Texture2D::SetImageFormat(u32 format)
{
    m_ImageFormat = format;
}

void Texture2D::SetPath(std::string path)
{
    m_FilePath = path;
}

void Texture2D::SetType(std::string type)
{
    m_Type = type;
}

// Texture2D::Texture2D(const std::string& path, const std::string& type)
//     : m_TextureID(0), m_Type(type), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BBP(0)
// {
//     // create a texture object
//     glGenTextures(1, &m_TextureID);
//     glBindTexture(GL_TEXTURE_2D, m_TextureID);
//
//     // set the texture wrapping parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//     // set the texture filtering parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//     // load image, create texture and generate mipmaps
//     stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded textures on the y-axis
//     m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BBP, 0);
//     if (!m_LocalBuffer)
//         LOG_ERROR("Texture: Failed to load {0}", path.c_str());
//
//     if (m_BBP == 1)
//         m_Format = GL_RED;
//     if (m_BBP == 3)
//         m_Format = GL_RGB;
//     if (m_BBP == 4)
//         m_Format = GL_RGBA;
//
//     glTexImage2D(GL_TEXTURE_2D, 0, m_Format, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, m_LocalBuffer);
//     glGenerateMipmap(GL_TEXTURE_2D);
//     glBindTexture(GL_TEXTURE_2D, 0);
//
//     if (m_LocalBuffer)
//         stbi_image_free(m_LocalBuffer);
// }
