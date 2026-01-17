//
//  CarDoorBody.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/11/26.
//

#ifndef CARDOORBODY_HPP
#define CARDOORBODY_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class CarDoorBody : public Object
{
public:
    CarDoorBody(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~CarDoorBody();

private:
    VAO CarDoorBodyVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
