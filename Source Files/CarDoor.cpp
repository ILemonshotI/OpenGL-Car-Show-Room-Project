//
//  CarDoor.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/11/26.
//
#include "CarDoor.hpp"
#include <glm/gtc/matrix_transform.hpp>

CarDoor::CarDoor(Shader* shader, Texture* texture,
    glm::vec3 position,
    glm::vec3 rotationAxis,
    float rotationSpeed)
    : Object(shader, position, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f)
{
    // Create car body (cube)
    body = new CarDoorBody(shader, texture, position, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f);
    body->useTexture = false;
    body->color = glm::vec4(0.22f, 1.0f, 0.08f, 1.0f);




    windowOffsets[0] = glm::vec3(0.0f, 0.3f, 0.0f);

    window = new CarWindow(
        shader,
        position + windowOffsets[0],
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f, 0.4f
    );

    window->color = glm::vec4(0.60f, 0.75f, 0.85f, 0.4f);
}

void CarDoor::toggle()
{
    isOpen = !isOpen;
    targetAngle = isOpen ? -70.0f : 0.0f; // realistic door angle
}

CarDoor::~CarDoor()
{
    delete body;
    delete window;
}

void CarDoor::update(float deltaTime)
{
    if (currentAngle < targetAngle)
        currentAngle = glm::min(currentAngle + openSpeed * deltaTime, targetAngle);
    else if (currentAngle > targetAngle)
        currentAngle = glm::max(currentAngle - openSpeed * deltaTime, targetAngle);

    body->update(deltaTime);
    window->update(deltaTime);
}




void CarDoor::draw(const glm::mat4& view, const glm::mat4& proj)
{

    body->useTexture = true;
    body->draw(view, proj);


    Material::setGlass(*shader, 4.0f, 0.15f); // strength and opacity
    window->draw(view, proj);
    Material::unSetGlass(*shader);
}
void CarDoor::drawWithParent(const glm::mat4& parentModel,
    const glm::mat4& view,
    const glm::mat4& proj)
{

    glm::vec3 hingePosition = position + glm::vec3(-0.3f, 0.0f, 0.0f);


    glm::mat4 doorTransform = parentModel;


    doorTransform = glm::translate(doorTransform, hingePosition);


    doorTransform = glm::rotate(
        doorTransform,
        glm::radians(currentAngle),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );


    doorTransform = glm::translate(doorTransform, -hingePosition);


    body->useTexture = 1;
    body->drawWithParent(doorTransform, view, proj);

    Material::setGlass(*shader, 4.0f, 0.15f);
    glEnable(GL_BLEND);                                  // Enable mixing colors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    // Standard transparency formula
    glDepthMask(GL_FALSE);


    window->drawWithParent(doorTransform, view, proj);
    window->useTexture = 0;

    glDepthMask(GL_TRUE);   // Re-enable depth writing for other objects
    glDisable(GL_BLEND);
    Material::unSetGlass(*shader);

   
    

}
