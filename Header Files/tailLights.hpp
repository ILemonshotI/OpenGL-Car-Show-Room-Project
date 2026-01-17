//
//  tailLights.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/14/26.
//

#ifndef TAILLIGHTS_HPP
#define TAILLIGHTS_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class tailLights : public Object
{
public:
    tailLights(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~tailLights();

private:
    VAO tailLightsVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
