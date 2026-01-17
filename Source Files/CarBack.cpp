//
//  CarBack.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#include "CarBack.hpp"


CarBack::CarBack(
    Shader* shader,
    Texture* texture,
    glm::vec3 position,
    glm::vec3 rotationAxis,
    float rotationSpeed,
    float alpha = 1.0f
)
    : Object(shader, texture, position, rotationAxis, rotationSpeed,alpha)
{
    setupMesh();

    // Assign the Cube VAO to base Object pointer
    this->vao = &CarBackVAO;
}

CarBack::~CarBack()
{
    CarBackVAO.Delete();
    vbo->Delete();
    ebo->Delete();
}

void CarBack::setupMesh()
{
    GLfloat vertices[] = {
        // FRONT FACE (normal 0,0,1)
        0.0f,-0.3f,0.6f,0.0f,0.0f,1.0f,0.0f, 0.0f,
         0.025f,-0.3f,0.6f,0.0f,0.0f,1.0f,1.0f,0.0f,
         0.025f,0.3f,0.6f,0.0f,0.0f,1.0f,1.0f,1.0f,
        0.0f, 0.3f,0.6f,0.0f,0.0f,1.0f,0.0f,1.0f,

        // RIGHT FACE (normal 1,0,0)
         0.025f, -0.3f,  0.6f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.025f, -0.3f, -0.6f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.025f, 0.3f, -0.6f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.025f, 0.3f,  0.6f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        // BACK FACE (FIXED NORMAL)
        0.025f, -0.3f, -0.6f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
        0.0f,  -0.3f, -0.6f,  0.0f, 0.0f,  1.0f,  1.0f, 0.0f,
        0.0f,   0.3f, -0.6f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
        0.025f, 0.3f, -0.6f,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f,


        // LEFT FACE (normal -1,0,0)
        0.0f, -0.3f, -0.6f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        0.0f, -0.3f,  0.6f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        0.0f, 0.3f,  0.6f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 0.3f, -0.6f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        // TOP FACE (normal 0,1,0)
        0.0f,0.3f,0.6f,0.0f, 1.0f, 0.0f,0.0f,0.0f,
         0.025f,0.3f,0.6f,0.0f,1.0f,0.0f,1.0f,0.0f,
         0.025f, 0.3f,-0.6f,0.0f,1.0f,0.0f,1.0f, 1.0f,
        0.0f,0.3f,-0.6f,0.0f,1.0f,0.0f,0.0f,1.0f,

        // BOTTOM FACE (normal 0,-1,0)
        0.0f, -0.3f, -0.6f,0.0f,-1.0f,0.0f,0.0f,0.0f,
         0.025f,-0.3f,-0.6f,0.0f,-1.0f,0.0f,1.0f,0.0f,
         0.025f,-0.3f,0.6f,0.0f,-1.0f,0.0f,1.0f,1.0f,
        0.0f,-0.3f,0.6f,0.0f,-1.0f,0.0f,0.0f,1.0f
    };


    GLuint indices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };
    
    CarBackVAO.Bind();

    vbo = new VBO(vertices, sizeof(vertices));
    ebo = new EBO(indices, sizeof(indices));

    CarBackVAO.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);    // position
    CarBackVAO.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normal
    CarBackVAO.LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texcoord


    CarBackVAO.Unbind();
}
