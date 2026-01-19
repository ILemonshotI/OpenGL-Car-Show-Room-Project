//
//  Cylinder.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/9/26.
//

#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <vector>

class Cylinder : public Object
{
public:
    Cylinder(
        float topRadius,
        float bottomRadius,
        float height,
        int segments,
        Shader* shader,
        Texture* texture,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f),
        float rotationSpeed = 0.0f,
        float alpha = 1.0f
    );

    ~Cylinder();

    void update(float deltaTime) override;
    void draw(const glm::mat4& view, const glm::mat4& proj) override;
    void drawWithParent(const glm::mat4& parent,
        const glm::mat4& view,
        const glm::mat4& proj) override;




private:
    void setupMesh();
    void generateVertices(float* vertices, int& vertexCount);

    float m_TopRadius, m_BottomRadius, m_Height;
    int m_SegmentCount;

    VAO cylinderVAO;
    VBO* vbo;
    EBO* ebo;
    int indexCount; // number of indices for glDrawElements
};

#endif
