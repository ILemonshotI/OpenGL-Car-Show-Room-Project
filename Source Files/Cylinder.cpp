//
//  Cylinder.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/9/26.
//

#include "Cylinder.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <iostream>


Cylinder::Cylinder(float topRadius, float bottomRadius, float height, int segments,
                   Shader* shader, Texture* texture,
                   glm::vec3 position, glm::vec3 rotationAxis,
                   float rotationSpeed, float alpha)
    : Object(shader, texture, position, rotationAxis, rotationSpeed, alpha),
      m_TopRadius(topRadius), m_BottomRadius(bottomRadius), m_Height(height),
      m_SegmentCount(segments), vbo(nullptr), ebo(nullptr), indexCount(0)
{
    setupMesh();
    this->vao = &cylinderVAO;
}

Cylinder::~Cylinder()
{
    cylinderVAO.Delete();
    if(vbo) vbo->Delete();
    if(ebo) ebo->Delete();
    delete vbo;
    delete ebo;
}

void Cylinder::setupMesh()
{
    // Estimate maximum number of vertices
    int vertexCapacity = m_SegmentCount * 3 * 2      // top & bottom triangles
                       + m_SegmentCount * 6;        // side triangles (2 per segment, 3 vertices each)

    std::vector<float> vertices(vertexCapacity * 8); // 8 floats per vertex (pos+normal+tex)
    int vertexCount = 0;

    generateVertices(vertices.data(), vertexCount);

    // Generate indices for glDrawElements
    indexCount = vertexCount / 8; // 1 index per vertex

    std::vector<unsigned int> indices(indexCount);
    for(int i = 0; i < indexCount; ++i) indices[i] = i;

    cylinderVAO.Bind();
    vbo = new VBO(vertices.data(), vertices.size() * sizeof(float));
    ebo = new EBO(indices.data(), indices.size() * sizeof(unsigned int));

    // Vertex attributes: position (3), normal (3), texcoord (2)
    cylinderVAO.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    cylinderVAO.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    cylinderVAO.LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    cylinderVAO.Unbind();
}

void Cylinder::draw(const glm::mat4& view, const glm::mat4& proj)
{
    if(!shader || !vao) return;

    shader->Activate();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    // rotate cylinder 90° around X so it lies along Z
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
    

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "proj"), 1, GL_FALSE, &proj[0][0]);

    glUniform1i(glGetUniformLocation(shader->ID, "useTexture"), useTexture ? 1 : 0);
    glm::vec4 finalColor = color;
    finalColor.a = alpha;
    glUniform4fv(glGetUniformLocation(shader->ID, "objectColor"), 1, &finalColor[0]);

    if(useTexture && texture) texture->Bind();

    vao->Bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    vao->Unbind();
}

void Cylinder::update(float deltaTime)
{
    rotationAngle += rotationSpeed * deltaTime;
    if(rotationAngle > 360.0f) rotationAngle -= 360.0f;
}


void Cylinder::generateVertices(float* vertices, int& vertexCount)
{
    float angleStep = 360.0f / m_SegmentCount;
    int index = 0;

    // Top circle
    for (int i = 0; i < m_SegmentCount; ++i) {
        float angle1 = glm::radians(i * angleStep);
        float angle2 = glm::radians((i + 1) % m_SegmentCount * angleStep);

        // Center vertex
        vertices[index++] = 0.0f; // x
        vertices[index++] = m_Height / 2.0f; // y
        vertices[index++] = 0.0f; // z
        vertices[index++] = 0.0f; vertices[index++] = 1.0f; vertices[index++] = 0.0f;
        vertices[index++] = 0.5f; vertices[index++] = 0.5f;

        // First edge vertex
        vertices[index++] = m_TopRadius * cos(angle1);
        vertices[index++] = m_Height / 2.0f;
        vertices[index++] = m_TopRadius * sin(angle1);
        vertices[index++] = 0.0f; vertices[index++] = 1.0f; vertices[index++] = 0.0f;
        vertices[index++] = 0.5f + 0.5f * cos(angle1);
        vertices[index++] = 0.5f + 0.5f * sin(angle1);

        // Second edge vertex
        vertices[index++] = m_TopRadius * cos(angle2);
        vertices[index++] = m_Height / 2.0f;
        vertices[index++] = m_TopRadius * sin(angle2);
        vertices[index++] = 0.0f; vertices[index++] = 1.0f; vertices[index++] = 0.0f;
        vertices[index++] = 0.5f + 0.5f * cos(angle2);
        vertices[index++] = 0.5f + 0.5f * sin(angle2);
    }

    // Bottom circle
    for (int i = 0; i < m_SegmentCount; ++i) {
        float angle1 = glm::radians(i * angleStep);
        float angle2 = glm::radians((i + 1) % m_SegmentCount * angleStep);

        // Center vertex
        vertices[index++] = 0.0f;
        vertices[index++] = -m_Height / 2.0f;
        vertices[index++] = 0.0f;
        vertices[index++] = 0.0f; vertices[index++] = -1.0f; vertices[index++] = 0.0f;
        vertices[index++] = 0.5f; vertices[index++] = 0.5f;

        // First edge vertex
        vertices[index++] = m_BottomRadius * cos(angle1);
        vertices[index++] = -m_Height / 2.0f;
        vertices[index++] = m_BottomRadius * sin(angle1);
        vertices[index++] = 0.0f; vertices[index++] = -1.0f; vertices[index++] = 0.0f;
        vertices[index++] = 0.5f + 0.5f * cos(angle1);
        vertices[index++] = 0.5f + 0.5f * sin(angle1);

        // Second edge vertex
        vertices[index++] = m_BottomRadius * cos(angle2);
        vertices[index++] = -m_Height / 2.0f;
        vertices[index++] = m_BottomRadius * sin(angle2);
        vertices[index++] = 0.0f; vertices[index++] = -1.0f; vertices[index++] = 0.0f;
        vertices[index++] = 0.5f + 0.5f * cos(angle2);
        vertices[index++] = 0.5f + 0.5f * sin(angle2);
    }

    // Side surface
    for (int i = 0; i < m_SegmentCount; ++i) {
        float angle1 = glm::radians(i * angleStep);
        float angle2 = glm::radians((i + 1) % m_SegmentCount * angleStep);

        float x1Top = m_TopRadius * cos(angle1);
        float z1Top = m_TopRadius * sin(angle1);
        float x2Top = m_TopRadius * cos(angle2);
        float z2Top = m_TopRadius * sin(angle2);

        float x1Bottom = m_BottomRadius * cos(angle1);
        float z1Bottom = m_BottomRadius * sin(angle1);
        float x2Bottom = m_BottomRadius * cos(angle2);
        float z2Bottom = m_BottomRadius * sin(angle2);

        // Triangle 1
        vertices[index++] = x1Bottom; vertices[index++] = -m_Height / 2.0f; vertices[index++] = z1Bottom;
        vertices[index++] = x1Bottom - x1Top; vertices[index++] = 0.0f; vertices[index++] = z1Bottom - z1Top;
        vertices[index++] = static_cast<float>(i) / m_SegmentCount; vertices[index++] = 0.0f;

        vertices[index++] = x1Top; vertices[index++] = m_Height / 2.0f; vertices[index++] = z1Top;
        vertices[index++] = x1Bottom - x1Top; vertices[index++] = 0.0f; vertices[index++] = z1Bottom - z1Top;
        vertices[index++] = static_cast<float>(i) / m_SegmentCount; vertices[index++] = 1.0f;

        vertices[index++] = x2Top; vertices[index++] = m_Height / 2.0f; vertices[index++] = z2Top;
        vertices[index++] = x2Bottom - x2Top; vertices[index++] = 0.0f; vertices[index++] = z2Bottom - z2Top;
        vertices[index++] = static_cast<float>(i + 1) / m_SegmentCount; vertices[index++] = 1.0f;

        // Triangle 2
        vertices[index++] = x1Bottom; vertices[index++] = -m_Height / 2.0f; vertices[index++] = z1Bottom;
        vertices[index++] = x1Bottom - x1Top; vertices[index++] = 0.0f; vertices[index++] = z1Bottom - z1Top;
        vertices[index++] = static_cast<float>(i) / m_SegmentCount; vertices[index++] = 0.0f;

        vertices[index++] = x2Top; vertices[index++] = m_Height / 2.0f; vertices[index++] = z2Top;
        vertices[index++] = x2Bottom - x2Top; vertices[index++] = 0.0f; vertices[index++] = z2Bottom - z2Top;
        vertices[index++] = static_cast<float>(i + 1) / m_SegmentCount; vertices[index++] = 1.0f;

        vertices[index++] = x2Bottom; vertices[index++] = -m_Height / 2.0f; vertices[index++] = z2Bottom;
        vertices[index++] = x2Bottom - x2Top; vertices[index++] = 0.0f; vertices[index++] = z2Bottom - z2Top;
        vertices[index++] = static_cast<float>(i + 1) / m_SegmentCount; vertices[index++] = 0.0f;
    }

    vertexCount = index;
}

void Cylinder::drawWithParent(const glm::mat4& parent,
                              const glm::mat4& view,
                              const glm::mat4& proj)
{
    if(!vao || !shader) return;

    shader->Activate();

    glm::mat4 model = parent;
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1,0,0));
    model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);

    glUniformMatrix4fv(glGetUniformLocation(shader->ID,"model"),1,GL_FALSE,&model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID,"view"),1,GL_FALSE,&view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID,"proj"),1,GL_FALSE,&proj[0][0]);

    // Tell shader to use texture
    glUniform1i(glGetUniformLocation(shader->ID,"useTexture"), useTexture ? 1 : 0);

    // Bind texture if needed
    if(useTexture && texture)
    {
        glActiveTexture(GL_TEXTURE0);           // activate texture unit 0
        texture->Bind();                        // binds the texture internally
        glUniform1i(glGetUniformLocation(shader->ID, "tex0"), 0); // tell shader tex0 = unit 0
    }

    vao->Bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    vao->Unbind();
}
