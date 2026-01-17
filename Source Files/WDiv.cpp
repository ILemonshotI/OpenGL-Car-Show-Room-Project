//
//  WDiv.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#include "WDiv.hpp"


WDiv::WDiv(
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
    this->vao = &WDivVAO;
}

WDiv::~WDiv()
{
    WDivVAO.Delete();
    vbo->Delete();
    ebo->Delete();

    delete vbo;
    delete ebo;
}


void WDiv::setupMesh()
{
    GLfloat vertices[] = {
        // FRONT FACE (normal 0,0,1)
        0.05f, 0.0f,  0.05f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.15f, 0.0f,  0.05f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.15f, 0.7f,  0.05f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.05f, 0.7f,  0.05f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

        // RIGHT FACE (normal 1,0,0)
         0.15f, 0.0f,  0.05f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.15f, 0.0f, -0.05f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.15f, 0.7f, -0.05f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.15f, 0.7f,  0.05f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        // 4BACK FACE (normal 0,0,-1)
         0.15f, 0.0f, -0.05f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
        0.05f, 0.0f, -0.05f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
        0.05f, 0.7f, -0.05f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
         0.15f, 0.7f, -0.05f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,

        // LEFT FACE (normal -1,0,0)
        0.05f, 0.0f, -0.05f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        0.05f, 0.0f,  0.05f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        0.05f, 0.7f,  0.05f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        0.05f, 0.7f, -0.05f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        // TOP FACE (normal 0,1,0)
        0.05f, 0.7f,  0.05f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
         0.15f, 0.7f,  0.05f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         0.15f, 0.7f, -0.05f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.05f, 0.7f, -0.05f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

        // BOTTOM FACE (normal 0,-1,0)
        0.05f, 0.0f, -0.05f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         0.15f, 0.0f, -0.05f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         0.15f, 0.0f,  0.05f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
        0.05f, 0.0f,  0.05f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f
    };


    GLuint indices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };
    WDivVAO.Bind();

    vbo = new VBO(vertices, sizeof(vertices));
    ebo = new EBO(indices, sizeof(indices));

    WDivVAO.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); // position
    WDivVAO.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normal
    WDivVAO.LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texcoord


    WDivVAO.Unbind();
}
