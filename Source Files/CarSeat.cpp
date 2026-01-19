//
//  CarSeat.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/15/26.
//

#include "CarSeat.hpp"
#include <vector>

constexpr GLuint FLOATS_PER_VERTEX = 8;
constexpr GLuint SEAT_VERTEX_COUNT = 24;
constexpr GLuint BACK_VERTEX_COUNT = 24;
constexpr GLuint HEAD_VERTEX_COUNT = 24;

CarSeat::CarSeat(
    Shader* shader,
    Texture* texture,
    glm::vec3 position,
    glm::vec3 rotationAxis,
    float rotationSpeed,
    float alpha
)
    : Object(shader, texture, position, rotationAxis, rotationSpeed, alpha)
{
    setupMesh();
    this->vao = &CarSeatVAO;
}

CarSeat::~CarSeat()
{
    CarSeatVAO.Delete();
    if (vbo) vbo->Delete();
    if (ebo) ebo->Delete();
}

void CarSeat::setupMesh()
{
    // ================= SEAT BOTTOM =================
    GLfloat seatBottomVertices[] = {
        0.0f,0.0f, 0.2f, 0,0,1, 0,0,
         0.4f,0.0f, 0.2f, 0,0,1, 1,0,
         0.4f,0.1f, 0.2f, 0,0,1, 1,1,
        0.0f,0.1f, 0.2f, 0,0,1, 0,1,

         0.4f,0.0f, 0.2f, 1,0,0, 0,0,
         0.4f,0.0f,-0.2f, 1,0,0, 1,0,
         0.4f,0.1f,-0.2f, 1,0,0, 1,1,
         0.4f,0.1f, 0.2f, 1,0,0, 0,1,

         0.4f,0.0f,-0.2f, 0,0,-1, 0,0,
        0.0f,0.0f,-0.2f, 0,0,-1, 1,0,
        0.0f,0.1f,-0.2f, 0,0,-1, 1,1,
         0.4f,0.1f,-0.2f, 0,0,-1, 0,1,

        0.0f,0.0f,-0.2f,-1,0,0, 0,0,
        0.0f,0.0f, 0.2f,-1,0,0, 1,0,
        0.0f,0.1f, 0.2f,-1,0,0, 1,1,
        0.0f,0.1f,-0.2f,-1,0,0, 0,1,

        0.0f,0.1f, 0.2f, 0,1,0, 0,0,
         0.4f,0.1f,0.2f, 0,1,0, 1,0,
         0.4f,0.1f,-0.2f, 0,1,0, 1,1,
        0.0f,0.1f,-0.2f, 0,1,0, 0,1,

        0.0,  0.0f,-0.2f, 0,-1,0, 0,0,
        0.4f, 0.0f,-0.2f, 0,-1,0, 1,0,
        0.4f, 0.0f, 0.2f, 0,-1,0, 1,1,
        0.0f, 0.0f, 0.2f, 0,-1,0, 0,1
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
        0.36f,0.0f, 0.2f, 0,0,1, 0,0,
         0.44f,0.0f, 0.2f, 0,0,1, 1,0,
         0.44f,0.5f, 0.2f, 0,0,1, 1,1,
        0.36f,  0.5f, 0.2f, 0,0,1, 0,1,

         0.44f,0.0f, 0.2f, 1,0,0, 0,0,
         0.44f,0.0f,-0.2f, 1,0,0, 1,0,
         0.44f,0.5f,-0.2f, 1,0,0, 1,1,
         0.44f,0.5f, 0.2f, 1,0,0, 0,1,

         0.44f,0.0f,-0.2f, 0,0,-1, 0,0,
        0.36f, 0.0f,-0.2f, 0,0,-1, 1,0,
        0.36f,  0.5f,-0.2f, 0,0,-1, 1,1,
         0.44f,0.5f,-0.2f, 0,0,-1, 0,1,

        0.36f,  0.0f,-0.2f,-1,0,0, 0,0,
        0.36f,  0.0f, 0.2f,-1,0,0, 1,0,
        0.36f,  0.5f, 0.2f,-1,0,0, 1,1,
        0.36f,  0.5f,-0.2f,-1,0,0, 0,1,

        0.36f,  0.5f, 0.2f, 0,1,0, 0,0,
         0.44f,0.5f, 0.2f, 0,1,0, 1,0,
         0.44f,0.5f,-0.2f, 0,1,0, 1,1,
        0.44f, 0.5f,-0.2f, 0,1,0, 0,1,

        0.36f,  0.0f,-0.2f, 0,-1,0, 0,0,
         0.44f, 0.0f,-0.2f, 0,-1,0, 1,0,
         0.44f, 0.0f, 0.2f, 0,-1,0, 1,1,
        0.36f,  0.0f, 0.2f, 0,-1,0, 0,1
    };

    // ================= HEADREST =================
    GLfloat headrestVertices[] = {
        0.36f,0.5f, 0.14f, 0,0,1, 0,0,
         0.44f,0.5f, 0.14f, 0,0,1, 1,0,
         0.44f,0.65f, 0.14f, 0,0,1, 1,1,
        0.36f,0.65f, 0.14f, 0,0,1, 0,1,

         0.44f,0.5f, 0.14f, 1,0,0, 0,0,
         0.44f,0.5f,-0.14f, 1,0,0, 1,0,
         0.44f,0.65f,-0.14f, 1,0,0, 1,1,
         0.44f,0.65f, 0.14f, 1,0,0, 0,1,

         0.44f,0.5f,-0.14f, 0,0,-1, 0,0,
        0.36f,0.5f,-0.14f, 0,0,-1, 1,0,
        0.36f,0.65f,-0.14f, 0,0,-1, 1,1,
         0.44f,0.65f,-0.14f, 0,0,-1, 0,1,

        0.36f,0.5f,-0.14f,-1,0,0, 0,0,
        0.36f,0.5f, 0.14f,-1,0,0, 1,0,
        0.36f,0.65f, 0.14f,-1,0,0, 1,1,
        0.36f,0.65f,-0.14f,-1,0,0, 0,1,

        0.36f,0.65f, 0.14f, 0,1,0, 0,0,
         0.44f,0.65f, 0.14f, 0,1,0, 1,0,
         0.44f,0.65f,-0.14f, 0,1,0, 1,1,
        0.36f,0.65f,-0.14f, 0,1,0, 0,1,

        0.36f,0.5f,-0.14f, 0,-1,0, 0,0,
         0.44f,0.5f,-0.14f, 0,-1,0, 1,0,
         0.44f,0.5f, 0.14f, 0,-1,0, 1,1,
        0.36f,0.5f, 0.14f, 0,-1,0, 0,1
    };

    // ================= MERGE =================
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    vertices.insert(vertices.end(), std::begin(seatBottomVertices), std::end(seatBottomVertices));
    vertices.insert(vertices.end(), std::begin(backrestVertices), std::end(backrestVertices));
    vertices.insert(vertices.end(), std::begin(headrestVertices), std::end(headrestVertices));

    indices.insert(indices.end(), std::begin(cubeIndices), std::end(cubeIndices));

    for (GLuint i : cubeIndices)
        indices.push_back(i + SEAT_VERTEX_COUNT);

    for (GLuint i : cubeIndices)
        indices.push_back(i + SEAT_VERTEX_COUNT + BACK_VERTEX_COUNT);

    // ================= GPU =================
    CarSeatVAO.Bind();

    vbo = new VBO(vertices.data(), vertices.size() * sizeof(GLfloat));
    ebo = new EBO(indices.data(), indices.size() * sizeof(GLuint));

    CarSeatVAO.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    CarSeatVAO.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    CarSeatVAO.LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    CarSeatVAO.Unbind();
}
