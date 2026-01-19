//
//  Texture.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/6/26.
//

#include "Texture.hpp"

Texture::Texture(const std::string& imagePath, GLenum texType, GLenum pixelType)
    : type(texType)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);

    if (!data)
    {
        std::cerr << "[Texture] Failed to load: " << imagePath << std::endl;
        // Generate a 1x1 white texture as fallback
        glGenTextures(1, &ID);
        glBindTexture(type, ID);
        unsigned char white[4] = { 255, 255, 255, 255 };
        glTexImage2D(type, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
        glBindTexture(type, 0);
        return;
    }

    // Detect format automatically
    GLenum format = (channels == 1) ? GL_RED :
        (channels == 3) ? GL_RGB :
        (channels == 4) ? GL_RGBA : GL_RGB;

    glGenTextures(1, &ID);
    glBindTexture(type, ID);

    // Texture filtering
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Upload texture
    glTexImage2D(type, 0, format, width, height, 0, format, pixelType, data);
    glGenerateMipmap(type);

    stbi_image_free(data);
    glBindTexture(type, 0);

    std::cout << "[Texture] Loaded: " << imagePath
        << " (" << width << "x" << height << ", " << channels << " channels)" << std::endl;
}

void Texture::Bind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(type, ID);
}

void Texture::Unbind() const
{
    glBindTexture(type, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}

