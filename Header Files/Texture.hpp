//
//  Texture.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/6/26.
//
#ifndef TEXTURE_CLASS_HPP
#define TEXTURE_CLASS_HPP

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <string>
#include <iostream>
#include "Shader.hpp"

class Texture
{
public:
    GLuint ID;
    GLenum type;

    // Constructor: load texture from file
    // texType defaults to GL_TEXTURE_2D
    // pixelType defaults to GL_UNSIGNED_BYTE
    Texture(const std::string& imagePath, GLenum texType = GL_TEXTURE_2D, GLenum pixelType = GL_UNSIGNED_BYTE);

    // Bind to texture unit (default 0)
    void Bind(GLuint unit = 0) const;

    // Unbind
    void Unbind() const;

    // Delete texture
    void Delete();
};

#endif
