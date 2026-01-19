//
//  emblem.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/18/26.
//

#ifndef EMBLEM_HPP
#define EMBLEM_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class emblem : public Object
{
public:
    emblem(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~emblem();

private:
    VAO emblemVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
