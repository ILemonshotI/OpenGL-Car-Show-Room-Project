//
//  WDiv.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#ifndef WDIV_HPP
#define WDIV_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class WDiv : public Object
{
public:
    WDiv(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~WDiv();

private:
    VAO WDivVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
