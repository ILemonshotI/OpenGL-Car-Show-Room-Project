//
//  Frame.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/19/26.
//

#ifndef FRAME_HPP
#define FRAME_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class Frame : public Object
{
public:
    Frame(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~Frame();

private:
    VAO FrameVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
