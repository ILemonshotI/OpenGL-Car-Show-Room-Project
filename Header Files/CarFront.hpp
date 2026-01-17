//
//  CarFloor.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#ifndef CARFRONT_HPP
#define CARFRONT_HPP

#include "Object.hpp"
#include "Cube.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class CarFront : public Object
{
public:
    CarFront(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~CarFront();
    
    


private:
    VAO CarFrontVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;
   

    void setupMesh();
};

#endif
