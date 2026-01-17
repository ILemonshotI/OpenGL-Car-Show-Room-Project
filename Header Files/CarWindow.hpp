//
//  CarWindow.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/11/26.
//

#ifndef CARWINDOW_HPP
#define CARWINDOW_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class CarWindow : public Object
{
public:
    CarWindow(
        Shader* shader,
        //Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~CarWindow();

private:
    VAO CarWindowVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;

    void setupMesh();
};

#endif
