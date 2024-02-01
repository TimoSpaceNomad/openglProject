#pragma once

#include "Renderer.h"

class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_FilePaht;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;

public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot);
    void Unbing();

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};