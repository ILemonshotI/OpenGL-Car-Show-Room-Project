//
//  BackWindow.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#ifndef BACKWINDOW_HPP
#define BACKWINDOW_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class BackWindow : public Object
{
public:
    BackWindow(
        Shader* shader,
        //Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~BackWindow();

private:
    VAO BackWindowVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
