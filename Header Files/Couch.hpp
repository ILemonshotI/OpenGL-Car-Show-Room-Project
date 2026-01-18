#ifndef COUCH_HPP
#define COUCH_HPP

#include "Object.hpp"
#include "ArmRest.hpp"
#include "Cube.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class Couch : public Object
{
public:
    Couch(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~Couch();

    void drawFull(const glm::mat4& view,
        const glm::mat4& proj);






private:
    VAO CouchVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;


    ArmRest* arms[2];
    glm::vec3 armsOffsets[2];



    void setupMesh();
};

#endif