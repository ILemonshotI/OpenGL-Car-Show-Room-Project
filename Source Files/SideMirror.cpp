//
//  SideMirror.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/14/26.
//

#include "SideMirror.hpp"


SideMirror::SideMirror(
    Shader* shader,
    Texture* texture,
    glm::vec3 position,
    glm::vec3 rotationAxis,
    float rotationSpeed,
    float alpha= 1.0f
)
    : Object(shader, texture, position, rotationAxis, rotationSpeed, alpha)
{
    setupMesh();

    // Assign the Cube VAO to base Object pointer
    this->vao = &SideMirrorVAO;
}

SideMirror::~SideMirror()
{
    SideMirrorVAO.Delete();
    vbo->Delete();
    ebo->Delete();

    
}


void SideMirror::setupMesh()
{
    GLfloat vertices[] = {
        // FRONT FACE (normal 0,0,1)
        0.0f, 0.0f,  0.1f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.05f, 0.0f,  0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.05f, 0.1f,  0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.0f, 0.1f,  0.1f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

        // RIGHT FACE (normal 1,0,0)
         0.05f, 0.0f,  0.1f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.05f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.05f, 0.1f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.05f, 0.1f,  0.01f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        // 4BACK FACE (normal 0,0,-1)
         0.05f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
        0.0f, 0.1f, 0.0f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
         0.05f, 0.1f, 0.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,

        // LEFT FACE (normal -1,0,0)
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        0.0f, 0.0f,  0.1f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        0.0f, 0.1f,  0.1f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 0.1f, 0.0f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        // TOP FACE (normal 0,1,0)
        0.0f, 0.1f,  0.1f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
         0.05f, 0.1f,  0.1f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         0.05f, 0.1f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 0.1f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

        // BOTTOM FACE (normal 0,-1,0)
        0.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         0.05f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         0.05f, 0.0f,  0.1f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 0.0f,  0.1f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f
    };


    GLuint indices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };
    SideMirrorVAO.Bind();

    vbo = new VBO(vertices, sizeof(vertices));
    ebo = new EBO(indices, sizeof(indices));

    SideMirrorVAO.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); // position
    SideMirrorVAO.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normal
    SideMirrorVAO.LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texcoord


    SideMirrorVAO.Unbind();
}
