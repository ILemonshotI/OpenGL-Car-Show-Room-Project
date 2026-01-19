//
//  Podium.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/19/26.
//

#ifndef PODIUM_HPP
#define PODIUM_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <vector>

class Podium : public Object
{
public:
    Podium(
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

    ~Podium();

    glm::vec3 getCalculatedBaseSize() const {
        return glm::vec3(m_BottomRadius * 2.0f, m_Height, m_BottomRadius * 2.0f);
    }
    void draw(const glm::mat4& view, const glm::mat4& proj) override;





private:
    void setupMesh();
    void generateVertices(float* vertices, int& vertexCount);

    float m_TopRadius, m_BottomRadius, m_Height;
    int m_SegmentCount;

    VAO PodiumVAO;
    VBO* vbo;
    EBO* ebo;
    int indexCount; // number of indices for glDrawElements
};

#endif
