#include "Texture.h"

#include <sstream>

#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
    : m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0) {


    GLCall(glGenTextures(32, m_RendererID));

    for (unsigned int i = 0; i < 32; ++i) {
        unsigned int value = 30 * i;

        std::ostringstream name;
        if (value < 10) {
            name << "00" << value << ".png";
        }
        else if ((10 <= value) && (value < 100)) {
            name << "0" << value << ".png";
        }
        else {
            name << value << ".png";
        }

        std::string filePath = path + name.str();

        LoadTexture(filePath, i);
    }
}

Texture::~Texture() {
    GLCall(glDeleteTextures(32, m_RendererID));
}

void Texture::Bind(unsigned int slot) {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID[slot]));
}

void Texture::Unbing() {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::LoadTexture(const std::string& filePath, unsigned int i) {
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID[i]));

    // Should be for all textures
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, m_LocalBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
}
