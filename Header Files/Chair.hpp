#ifndef CHAIR_HPP
#define CHAIR_HPP

#include "Object.hpp"
#include "Leg.hpp"
#include "Cube.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class Chair : public Object
{
public:
    Chair(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~Chair();

    void drawFull(const glm::mat4& view,
        const glm::mat4& proj);






private:
    VAO ChairVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;


    Leg* Legs[4];
    glm::vec3 LegsOffsets[4];



    void setupMesh();
};

#endif