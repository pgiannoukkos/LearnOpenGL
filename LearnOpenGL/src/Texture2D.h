#pragma once

#include "defines.h"

#include <glad/glad.h>

#include <string>

class Texture2D
{
public:
    Texture2D();

    // Generates texture from image data
    void Generate(u32 width, u32 height, const void* data, b8 mipmap = false);

    // Generates cubemap texture from image data
    void GenerateCubemap(u32 width, u32 height, b8 mipmap = false);

    void Bind(u32 slot = 0) const;
    void BindCubemap(u32 slot = 0) const;
    void UnBind();
    void UnBindCubemap();

    void Destroy();

    u32 GetTexID() const;
    u32 GetWidth() const;
    u32 GetHeight() const;
    std::string GetType() const;
    std::string GetPath() const;

    void SetInternalFormat(u32 format);
    void SetImageFormat(u32 format);
    void SetType(std::string type);
    void SetPath(std::string path);

private:
    u32 m_TextureID;
    u32 m_Width, m_Height;

    // Texture Format
    u32 m_InternalFormat; // format of texture object
    u32 m_ImageFormat;    // format of loaded image

    // Texture Configuration
    u32 m_WrapS;     // wrapping mode on S axis
    u32 m_WrapT;     // wrapping mode on T axis
    u32 m_WrapR;     // wrapping mode on R axis
    u32 m_FilterMin; // filtering mode if texture pixels < screen pixels
    u32 m_FilterMag; // filtering mode if texture pixels > screen pixels
    u32 m_DataType;

    std::string m_Type;
    std::string m_FilePath;

    // u8* m_LocalBuffer;
    // GLenum m_Format;
};
