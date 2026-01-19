//
//  Pedal.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#ifndef PEDAL_HPP
#define PEDAL_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class Pedal : public Object
{
public:
    Pedal(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~Pedal();

private:
    VAO PedalVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;



    void setupMesh();
};

#endif
