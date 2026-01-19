//
//  CarWindShield.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#include "Pedal.hpp"


Pedal::Pedal(
    Shader* shader,
    Texture* texture,
    glm::vec3 position,
    glm::vec3 rotationAxis,
    float rotationSpeed,
    float alpha
)
    : Object(shader, position, rotationAxis, rotationSpeed, alpha)
{
    setupMesh();
    this->vao = &PedalVAO;
}


Pedal::~Pedal()
{
    PedalVAO.Delete();
    vbo->Delete();
    ebo->Delete();

    delete vbo;
    delete ebo;
}

void Pedal::setupMesh()
{
    GLfloat vertices[] = {
        // FRONT FACE (normal 0,0,1) pedal
         0.0f, 0.09f,    0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        0.025f, 0.0f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.025f, 0.0f,  0.035f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.0f, 0.09f,  0.035f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

        // RIGHT FACE (normal 1,0,0) pedal
        0.0f, 0.09f,    0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.0f, 0.0f,    0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        0.025f, 0.0f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


        // BACK FACE (normal 0,0,-1) pedal
        0.0f, 0.09f,    0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        0.0f, 0.0f,    0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        0.0f, 0.09f,  0.035f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        0.0f, 0.0f,   0.035f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

        // LEFT FACE (normal -1,0,0) pedal
        0.0f, 0.09f,  0.035f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        0.0f, 0.0f,  0.035f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        0.025f, 0.0f,  0.035f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,



        // BOTTOM FACE (normal 0,-1,0) pedal
        0.0f, 0.0f,    0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        0.025f, 0.0f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        0.025f, 0.0f,  0.035f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.0f, 0.0f,  0.035f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f
    };


    GLuint indices[] = {
        0,1,2, 2,3,0,
        4,5,6,
        7,8,9, 9,8,10,
        11,12,13,
        14,15,16,
        14,16,17

    };

    PedalVAO.Bind();

    vbo = new VBO(vertices, sizeof(vertices));
    ebo = new EBO(indices, sizeof(indices));

    PedalVAO.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);    // position
    PedalVAO.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normal
    PedalVAO.LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texcoord


    PedalVAO.Unbind();
}
