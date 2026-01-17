//
//  MidDiv.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#ifndef MIDDIV_HPP
#define MIDDIV_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class MidDiv : public Object
{
public:
    MidDiv(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~MidDiv();

private:
    VAO MidDivVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
