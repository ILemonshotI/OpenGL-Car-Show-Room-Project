//
//  Tcentre.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/18/26.
//

#ifndef TCENTRE_HPP
#define TCENTRE_HPP

#include "Object.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class Tcentre : public Object
{
public:
    Tcentre(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~Tcentre();
    void setupMesh();

private:
    VAO TcentreVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;


};

#endif
