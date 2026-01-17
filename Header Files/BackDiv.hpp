//
//  BackDiv.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/14/26.
//

#ifndef BACKDIV_HPP
#define BACKDIV_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class BackDiv : public Object
{
public:
    BackDiv(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~BackDiv();

private:
    VAO BackDivVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
