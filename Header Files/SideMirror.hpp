//
//  SideMirror.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/14/26.
//

#ifndef SIDEMIRROR_HPP
#define SIDEMIRROR_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class SideMirror : public Object
{
public:
    SideMirror(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~SideMirror();

private:
    VAO SideMirrorVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
