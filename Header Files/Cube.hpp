//
//  cube.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/9/26.
//

#ifndef CUBE_HPP
#define CUBE_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class Cube : public Object
{
public:
    Cube(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~Cube();

private:
    VAO cubeVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
