#include "Texture.h"

#include "Log.h"

#include <iostream>
#include <stb_image.h>

Texture::Texture(const std::string& path, const std::string& type)
    : m_TextureID(0), m_Type(type), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BBP(0)
{
    // create a texture object
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded textures on the y-axis
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BBP, 0);
    if (!m_LocalBuffer)
        LOG_ERROR("Texture: Failed to load {0}", path.c_str());

    if (m_BBP == 1)
        m_Format = GL_RED;
    if (m_BBP == 3)
        m_Format = GL_RGB;
    if (m_BBP == 4)
        m_Format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, m_Format, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, m_LocalBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_TextureID);
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
