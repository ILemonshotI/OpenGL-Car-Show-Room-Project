//
//  CarSeat.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/15/26.
//

#ifndef CARSEAT_HPP
#define CARSEAT_HPP

#include "Object.hpp"
#include "Cube.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class CarSeat : public Object
{
public:
    CarSeat(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~CarSeat();






private:
    VAO CarSeatVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;


    void setupMesh();
};

#endif
