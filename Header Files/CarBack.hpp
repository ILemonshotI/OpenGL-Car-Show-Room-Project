//
//  CarBack.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#ifndef CARBACK_HPP
#define CARBACK_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class CarBack : public Object
{
public:
    CarBack(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~CarBack();

private:
    VAO CarBackVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
