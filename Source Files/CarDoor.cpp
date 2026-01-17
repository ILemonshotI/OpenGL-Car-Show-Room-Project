//
//  CarDoor.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/11/26.
//
#include "CarDoor.hpp"
#include <glm/gtc/matrix_transform.hpp>

CarDoor::CarDoor(Shader* shader,Texture* texture,
         glm::vec3 position,
         glm::vec3 rotationAxis,
         float rotationSpeed)
    : Object(shader, position, glm::vec3(0.0f,0.0f,0.0f), 0.0f)
{
    // Create car body (cube)
    body = new CarDoorBody(shader, texture, position, glm::vec3(0.0f,1.0f,0.0f), 0.0f, 1.0f);
    body->useTexture = false;
    body->color = glm::vec4(0.22f, 1.0f, 0.08f, 1.0f);
    


    // Define wheel offsets relative to cube
    windowOffsets[0] = glm::vec3(0.0f, 0.3f,  0.0f); // front-left
    //wheelOffsets[1] = glm::vec3( 0.6f, -0.2f,  0.6f); // front-right
    //wheelOffsets[2] = glm::vec3(-0.6f, -0.2f, -0.6f); // back-left
    //wheelOffsets[3] = glm::vec3( 0.6f, -0.2f, -0.6f); // back-right

    // Create wheels
   // for(int i = 0; i < 4; i++)
    //{
        window = new CarWindow(
            shader,
            position + windowOffsets[0],
            glm::vec3(0.0f, 1.0f, 0.0f), // y-axis for spinning
            0.0f,0.4f                     // wheel rotation speed
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
   
    body->useTexture=true;
    body->draw(view, proj);

  
    Material::setGlass(*shader, 4.0f, 0.15f); // strength and opacity
    window->draw(view, proj);
    Material::unSetGlass(*shader);
}
void CarDoor::drawWithParent(const glm::mat4& parentModel,
                             const glm::mat4& view,
                             const glm::mat4& proj)
{
    // 1️⃣ Define hinge in CAR-LOCAL space (fixed point)
    glm::vec3 hingePosition = position + glm::vec3(-0.3f, 0.0f, 0.0f);

    // 2️⃣ Build door transform relative to parent, pivoting around hinge
    glm::mat4 doorTransform = parentModel;

    // Translate to hinge (so hinge is at origin)
    doorTransform = glm::translate(doorTransform, hingePosition);

    // Rotate door around hinge
    doorTransform = glm::rotate(
        doorTransform,
        glm::radians(currentAngle),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Translate back so door's local position is restored
    doorTransform = glm::translate(doorTransform, -hingePosition);

    // 3️⃣ Draw door and window using this transform
    body->useTexture = 1;
    body->drawWithParent(doorTransform, view, proj);

    Material::setGlass(*shader, 4.0f, 0.15f);
    window->useTexture = 0;
    window->drawWithParent(doorTransform, view, proj);
    Material::unSetGlass(*shader);
}
