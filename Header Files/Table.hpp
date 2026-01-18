//
//  Table.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/17/26.
//

#ifndef TABLE_HPP
#define TABLE_HPP

#include "Object.hpp"
#include "Leg.hpp"
#include "Cube.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class Table : public Object
{
public:
    Table(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        glm::vec3 rotationAxis,
        float rotationSpeed,
        float alpha
    );

    ~Table();
    
    void drawFull( const glm::mat4& view,
                  const glm::mat4& proj);
                                

    
    


private:
    VAO TableVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;
    
    
    Leg* Legs[4];
    glm::vec3 LegsOffsets[4];
    
   

    void setupMesh();
};

#endif
