//
//  ArmRest.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/17/26.
//

#ifndef ARMREST_HPP
#define ARMREST_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class ArmRest : public Object
{
public:
    ArmRest(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~ArmRest();

private:
    VAO ArmRestVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
