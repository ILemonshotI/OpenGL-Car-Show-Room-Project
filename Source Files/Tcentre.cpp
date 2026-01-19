//
//  Tcentre
//  OpenGLProject
//
//  Created by Ameen on 1/18/26.
//

#include "Tcentre.hpp"
#include <vector>

constexpr GLuint FLOATS_PER_VERTEX = 8;
constexpr GLuint SEAT_VERTEX_COUNT = 24;
constexpr GLuint BACK_VERTEX_COUNT = 24;


Tcentre::Tcentre(
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

    // Assign the Cube VAO to base Object pointer
    this->vao = &TcentreVAO;
}

Tcentre::~Tcentre()
{
    TcentreVAO.Delete();
    vbo->Delete();
    ebo->Delete();


}

void Tcentre::setupMesh()
{

    GLfloat seatBottomVertices[] = {
        // +X FACE (main face)
         0.01f,-0.01f,  0.12f, 1,0,0, 0,0,
         0.01f, 0.01f,  0.12f, 1,0,0, 1,0,
         0.01f, 0.01f, -0.12f, 1,0,0, 1,1,
         0.01f,-0.01f, -0.12f, 1,0,0, 0,1,

         // -X FACE
         -0.01f,-0.01f, -0.12f,-1,0,0, 0,0,
         -0.01f, 0.01f, -0.12f,-1,0,0, 1,0,
         -0.01f, 0.01f,  0.12f,-1,0,0, 1,1,
         -0.01f,-0.01f,  0.12f,-1,0,0, 0,1,

         // +Y
         -0.01f, 0.01f,  0.12f, 0,1,0, 0,0,
          0.01f, 0.01f,  0.12f, 0,1,0, 1,0,
          0.01f, 0.01f, -0.12f, 0,1,0, 1,1,
         -0.01f, 0.01f, -0.12f, 0,1,0, 0,1,

         // -Y
         -0.01f,-0.01f, -0.12f, 0,-1,0, 0,0,
          0.01f,-0.01f, -0.12f, 0,-1,0, 1,0,
          0.01f,-0.01f,  0.12f, 0,-1,0, 1,1,
         -0.01f,-0.01f,  0.12f, 0,-1,0, 0,1,

         // +Z
         -0.01f,-0.01f,  0.12f, 0,0,1, 0,0,
          0.01f,-0.01f,  0.12f, 0,0,1, 1,0,
          0.01f, 0.01f,  0.12f, 0,0,1, 1,1,
         -0.01f, 0.01f,  0.12f, 0,0,1, 0,1,

         // -Z
          0.01f,-0.01f, -0.12f, 0,0,-1, 0,0,
         -0.01f,-0.01f, -0.12f, 0,0,-1, 1,0,
         -0.01f, 0.01f, -0.12f, 0,0,-1, 1,1,
          0.01f, 0.01f, -0.12f, 0,0,-1, 0,1
    };



    GLuint cubeIndices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };


    GLfloat backrestVertices[] = {
        // FRONT
        -0.01f,-0.12f,  0.01f, 0,0,1, 0,0,
         0.01f,-0.12f,  0.01f, 0,0,1, 1,0,
         0.01f, -0.01f,  0.01f, 0,0,1, 1,1,
        -0.01f, -0.01f,  0.01f, 0,0,1, 0,1,

        // RIGHT
         0.01f,-0.12f,  0.01f, 1,0,0, 0,0,
         0.01f,-0.12f, -0.01f, 1,0,0, 1,0,
         0.01f, -0.01f, -0.01f, 1,0,0, 1,1,
         0.01f, -0.01f,  0.01f, 1,0,0, 0,1,

         // BACK
          0.01f,-0.12f, -0.01f, 0,0,-1, 0,0,
         -0.01f,-0.12f, -0.01f, 0,0,-1, 1,0,
         -0.01f, -0.01f, -0.01f, 0,0,-1, 1,1,
          0.01f, -0.01f, -0.01f, 0,0,-1, 0,1,

          // LEFT
          -0.01f,-0.12f, -0.01f,-1,0,0, 0,0,
          -0.01f,-0.12f,  0.01f,-1,0,0, 1,0,
          -0.01f, -0.01f,  0.01f,-1,0,0, 1,1,
          -0.01f, -0.01f, -0.01f,-1,0,0, 0,1,

          // TOP
          -0.01f, -0.01f,  0.01f, 0,1,0, 0,0,
           0.01f, -0.01f,  0.01f, 0,1,0, 1,0,
           0.01f, -0.01f, -0.01f, 0,1,0, 1,1,
          -0.01f, -0.01f, -0.01f, 0,1,0, 0,1,

          // BOTTOM
          -0.01f,-0.12f, -0.01f, 0,-1,0, 0,0,
           0.01f,-0.12f, -0.01f, 0,-1,0, 1,0,
           0.01f,-0.12f,  0.01f, 0,-1,0, 1,1,
          -0.01f,-0.12f,  0.01f, 0,-1,0, 0,1
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
    TcentreVAO.Bind();

    vbo = new VBO(vertices.data(), vertices.size() * sizeof(GLfloat));
    ebo = new EBO(indices.data(), indices.size() * sizeof(GLuint));

    TcentreVAO.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    TcentreVAO.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    TcentreVAO.LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    TcentreVAO.Unbind();
}
