//
//  CarDoor.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/11/26.
//

#ifndef CARDOOR_HPP
#define CARDOOR_HPP

#include "Object.hpp"
#include "CarWindow.hpp"
#include "CarDoorBody.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"


class CarDoor : public Object
{
public:
    CarDoor(Shader* shader,Texture* texture,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f),
    float rotationSpeed = 0.0f) ;


    ~CarDoor();
    void update(float deltaTime) override;
    void draw(const glm::mat4& view, const glm::mat4& proj) override;
    virtual void drawWithParent(const glm::mat4& parentModel,
                                const glm::mat4& view,
                                const glm::mat4& proj) override;

    void toggle();



private:
    VAO CarDoorVAO;   // renamed to avoid shadowing
    VBO* vbo;
    EBO* ebo;
    glm::vec3 windowOffsets[4];
    CarWindow* window;
    CarDoorBody* body;
    bool isOpen = false;
    float currentAngle = 0.0f;      // degrees
    float targetAngle = 0.0f;       // degrees
    float openSpeed = 90.0f;        // deg/sec

 


    void setupMesh();
};

#endif
