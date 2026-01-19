//
//  Couch.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/17/26.
//

#include "Couch.hpp"
#include <vector>

constexpr GLuint FLOATS_PER_VERTEX = 8;
constexpr GLuint SEAT_VERTEX_COUNT = 24;
constexpr GLuint BACK_VERTEX_COUNT = 24;


Couch::Couch(
    Shader* shader,
    Texture* texture,
    glm::vec3 position,
    glm::vec3 rotationAxis,
    float rotationSpeed,
    float alpha
)
    : Object(shader, texture, position, rotationAxis, rotationSpeed, alpha)

{
    armsOffsets[0] = glm::vec3(-0.1f, -0.1f, 0.1f);
    armsOffsets[1] = glm::vec3(1.0f, -0.1f, 0.1f);


    for (int i = 0; i < 2; i++) {

        arms[i] = new ArmRest(shader,
            texture,
            armsOffsets[i],
            glm::vec3(0, 1, 0),
            0.0f,
            alpha);

    }

    setupMesh();
    this->vao = &CouchVAO;
}

Couch::~Couch()
{
    CouchVAO.Delete();
    if (vbo) vbo->Delete();
    if (ebo) ebo->Delete();
}

void Couch::setupMesh()
{
    // ================= SEAT BOTTOM =================
    GLfloat seatBottomVertices[] = {
        0.0f,-0.2f, 0.2f, 0,0,1, 0,0,
         1.0f,-0.2f, 0.2f, 0,0,1, 1,0,
         1.0f,0.1f, 0.2f, 0,0,1, 1,1,
        0.0f,0.1f, 0.2f, 0,0,1, 0,1,

         1.0f,-0.2f, 0.2f, 1,0,0, 0,0,
         1.0f,-0.2f,-0.2f, 1,0,0, 1,0,
         1.0f,0.1f,-0.2f, 1,0,0, 1,1,
         1.0f,0.1f, 0.2f, 1,0,0, 0,1,

         1.0f,-0.2f,-0.2f, 0,0,-1, 0,0,
        0.0f,-0.2f,-0.2f, 0,0,-1, 1,0,
        0.0f,0.1f,-0.2f, 0,0,-1, 1,1,
         1.0f,0.1f,-0.2f, 0,0,-1, 0,1,

        0.0f,-0.2f,-0.2f,-1,0,0, 0,0,
        0.0f,-0.2f, 0.2f,-1,0,0, 1,0,
        0.0f,0.1f, 0.2f,-1,0,0, 1,1,
        0.0f,0.1f,-0.2f,-1,0,0, 0,1,

        0.0f,0.1f, 0.2f, 0,1,0, 0,0,
         1.0f,0.1f,0.2f, 0,1,0, 1,0,
         1.0f,0.1f,-0.2f, 0,1,0, 1,1,
        0.0f,0.1f,-0.2f, 0,1,0, 0,1,

        0.0,  -0.2f,-0.2f, 0,-1,0, 0,0,
        1.0f, -0.2f,-0.2f, 0,-1,0, 1,0,
        1.0f, -0.2f, 0.2f, 0,-1,0, 1,1,
        0.0f, -0.2f, 0.2f, 0,-1,0, 0,1
    };

    GLuint cubeIndices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };

    // ================= BACKREST =================
    GLfloat backrestVertices[] = {
        0.0f,0.0f, -0.1f, 0,0,1, 0,0,
         1.0f,0.0f, -0.1f, 0,0,1, 1,0,
         1.0f,0.7f, -0.1f, 0,0,1, 1,1,
        0.0f,0.7f, -0.1f, 0,0,1, 0,1,

         1.0f,0.0f, -0.1f, 1,0,0, 0,0,
         1.0f,0.0f,-0.2f, 1,0,0, 1,0,
         1.0f,0.7f,-0.2f, 1,0,0, 1,1,
         1.0f,0.7f, -0.1f, 1,0,0, 0,1,

         1.0f,0.0f,-0.2f, 0,0,-1, 0,0,
        0.0f,0.0f,-0.2f, 0,0,-1, 1,0,
        0.0f,0.7f,-0.2f, 0,0,-1, 1,1,
         1.0f,0.7f,-0.2f, 0,0,-1, 0,1,

        0.0f,0.0f,-0.2f,-1,0,0, 0,0,
        0.0f,0.0f, -0.1f,-1,0,0, 1,0,
        0.0f,0.7f, -0.1f,-1,0,0, 1,1,
        0.0f,0.7f,-0.2f,-1,0,0, 0,1,

        0.0f,0.7f, -0.1f, 0,1,0, 0,0,
         1.0f,0.7f,-0.1f, 0,1,0, 1,0,
         1.0f,0.7f,-0.2f, 0,1,0, 1,1,
        0.0f,0.7f,-0.2f, 0,1,0, 0,1,

        0.0,  0.0f,-0.2f, 0,-1,0, 0,0,
        1.0f, 0.0f,-0.2f, 0,-1,0, 1,0,
        1.0f, 0.0f, -0.1f, 0,-1,0, 1,1,
        0.0f, 0.0f, -0.1f, 0,-1,0, 0,1
    };


    // ================= MERGE =================
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    vertices.insert(vertices.end(), std::begin(seatBottomVertices), std::end(seatBottomVertices));
    vertices.insert(vertices.end(), std::begin(backrestVertices), std::end(backrestVertices));


    indices.insert(indices.end(), std::begin(cubeIndices), std::end(cubeIndices));

    for (GLuint i : cubeIndices)
        indices.push_back(i + SEAT_VERTEX_COUNT);

    for (GLuint i : cubeIndices)
        indices.push_back(i + SEAT_VERTEX_COUNT + BACK_VERTEX_COUNT);

    // ================= GPU =================
    CouchVAO.Bind();

    vbo = new VBO(vertices.data(), vertices.size() * sizeof(GLfloat));
    ebo = new EBO(indices.data(), indices.size() * sizeof(GLuint));

    CouchVAO.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    CouchVAO.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    CouchVAO.LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    CouchVAO.Unbind();
}
void Couch::drawFull(const glm::mat4& view,
    const glm::mat4& proj)
{
    this->useTexture = true;

    // 1. Draw the couch normally (root object)
    this->draw(view, proj);

    // 2. Compute the couch model matrix ONCE
    glm::mat4 couchModel = this->getModelMatrix();

    // 3. Draw arms relative to the couch
    for (int i = 0; i < 2; i++)
    {
        arms[i]->useTexture = true;

        arms[i]->drawWithParent(
            couchModel,  // ? parent model matrix
            view,        // ? same camera view
            proj         // ? same projection
        );
    }
}
