//
//  CarBottom.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#ifndef CARBOTTOM_HPP
#define CARBOTTOM_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class CarBottom : public Object
{
public:
    CarBottom(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~CarBottom();

private:
    VAO CarBottomVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
