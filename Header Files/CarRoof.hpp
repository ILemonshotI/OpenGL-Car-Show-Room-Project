//
//  CarRoof.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#ifndef CARROOF_HPP
#define CARROOF_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class CarRoof : public Object
{
public:
    CarRoof(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~CarRoof();

private:
    VAO CarRoofVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
