//
//  HeadLights.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/14/26.
//

#ifndef HEADLIGHTS_HPP
#define HEADLIGHTS_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class HeadLights : public Object
{
public:
    HeadLights(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~HeadLights();

private:
    VAO HeadLightsVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
