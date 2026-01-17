//
//  CarWindShield.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#ifndef CARWINDSHIELD_HPP
#define CARWINDSHIELD_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class CarWindShield : public Object
{
public:
    CarWindShield(
        Shader* shader,
        //Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~CarWindShield();

private:
    VAO CarWindShieldVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;
    float tiltAngle; // degrees


    void setupMesh();
};

#endif
