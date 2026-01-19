//
//  Table.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/17/26.
//

#include "Table.hpp"

Table::Table(
    Shader* shader,
    Texture* texture,
    glm::vec3 position,
    glm::vec3 rotationAxis,
    float rotationSpeed,
    float alpha
)
    : Object(shader, texture, position, rotationAxis, rotationSpeed, alpha)

{
    LegsOffsets[0] = glm::vec3(0.0f, -0.4f, 0.14f);
    LegsOffsets[1] = glm::vec3(0.38f, -0.4f, 0.14f);
    LegsOffsets[2] = glm::vec3(0.0f, -0.4f, -0.2f);
    LegsOffsets[3] = glm::vec3(0.38f, -0.4f, -0.2f);

    for (int i = 0; i < 4; i++) {

        Legs[i] = new Leg(shader,
            texture,
            LegsOffsets[i],
            glm::vec3(0, 1, 0),
            0.0f,
            alpha);

    }

    setupMesh();
    // Assign the Cube VAO to base Object pointer
    this->vao = &TableVAO;
}

Table::~Table()
{
    TableVAO.Delete();
    vbo->Delete();
    ebo->Delete();

    delete vbo;
    delete ebo;
}

void Table::setupMesh()
{
    GLfloat vertices[] = {
        // FRONT FACE (normal 0,0,1)
        0.0f, 0.0f,  0.2f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.44f, 0.0f,  0.2f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.44f, 0.05f,  0.2f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.0f, 0.05f,  0.2f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

        // RIGHT FACE (normal 1,0,0)
         0.44f, 0.0f,  0.2f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.44f, 0.0f, -0.2f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.44f, 0.05f, -0.2f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.44f, 0.05f,  0.2f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

         // BACK FACE (normal 0,0,-1)
          0.44f, 0.0f, -0.2f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
         0.0f, 0.0f, -0.2f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
         0.0f, 0.05f, -0.2f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
          0.44f, 0.05f, -0.2f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,

          // LEFT FACE (normal -1,0,0)
          0.0f, 0.0f, -0.2f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
          0.0f, 0.0f,  0.2f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
          0.0f, 0.05f,  0.2f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
          0.0f, 0.05f, -0.2f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

          // TOP FACE (normal 0,1,0)
          0.0f, 0.05f,  0.2f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
           0.44f, 0.05f,  0.2f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
           0.44f, 0.05f, -0.2f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
          0.0f, 0.05f, -0.2f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

          // BOTTOM FACE (normal 0,-1,0)
          0.0f, 0.0f, -0.2f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
           0.44f, 0.0f, -0.2f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
           0.44f, 0.0f,  0.2f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
          0.0f, 0.0f,  0.2f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f
    };


    GLuint indices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };

    TableVAO.Bind();

    vbo = new VBO(vertices, sizeof(vertices));
    ebo = new EBO(indices, sizeof(indices));

    TableVAO.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);    // position
    TableVAO.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normal
    TableVAO.LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texcoord


    TableVAO.Unbind();
}
void Table::drawFull(const glm::mat4& view,
    const glm::mat4& proj)
{
    this->useTexture = true;

    // 1. Draw the couch normally (root object)
    this->draw(view, proj);

    // 2. Compute the couch model matrix ONCE
    glm::mat4 tableModel = this->getModelMatrix();

    // 3. Draw arms relative to the couch
    for (int i = 0; i < 4; i++)
    {
        Legs[i]->useTexture = true;

        Legs[i]->drawWithParent(
            tableModel,  //  parent model matrix
            view,        //  same camera view
            proj         // same projection
        );
    }
}
