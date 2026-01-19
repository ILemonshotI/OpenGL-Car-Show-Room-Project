//
//  car.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/9/26.
//

#ifndef CAR_HPP
#define CAR_HPP

#include "Object.hpp"
#include <GLFW/glfw3.h>

#include "Cube.hpp"
#include "Cylinder.hpp"
#include "CarDoor.hpp"
#include "CarFront.hpp"
#include "CarSeat.hpp"
#include "CarBottom.hpp"
#include "WDiv.hpp"
#include "MidDiv.hpp"
#include "BackDiv.hpp"
#include "CarRoof.hpp"
#include "BackWindow.hpp"
#include "CarBack.hpp"
#include "SideMirror.hpp"
#include "tailLights.hpp"
#include "HeadLights.hpp"
#include "Pedal.hpp"
#include "SteeringWheel.hpp"
#include "emblem.hpp"


class Car : public Object
{
public:
    Car(Shader* shader, Texture* frontTexture, Texture* wheelTexture, Texture* leather, Texture* stwheel, Texture* logo, glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f), float rotationSpeed = 0.0f, float alpha = 1.0f);

    ~Car();

    void update(float deltaTime) override;
    void draw(const glm::mat4& view, const glm::mat4& proj) override;
    void ToggleDriverDoor(const glm::vec3& cameraPos);

    bool isDriven = false;
    bool isDriverDoorOpen() const;

    glm::vec3 driverCameraOffset = glm::vec3(-0.25f, 0.35f, 0.0f);

    float driveSpeed = 3.0f;
    float turnSpeed = 60.0f;

    void setDriven(bool state);
    void drive(GLFWwindow* window, float dt);
    glm::vec3 getForward() const;
    glm::vec3 getDriverWorldPosition() const;
    float steeringAngle = 0.0f;
    float maxSteering = 90.0f;
private:
    CarFront* front;
    CarBack* back;
    CarBottom* bottom;
    CarRoof* roof;
    SteeringWheel* steering;
    Cylinder* wheels[4];
    CarDoor* CarDoors[4];
    WDiv* divider[4];
    MidDiv* div[2];
    BackDiv* backdiv[2];
    BackWindow* window[3];
    SideMirror* sideMirror[2];
    tailLights* TailLights[2];
    HeadLights* headLights[2];
    emblem* emblems[2];
    Pedal* pedal[2];
    CarSeat* carseats[4];

    Texture* wheelTexture;
    Texture* frontTexture;
    Texture* leather;
    Texture* stwheel;
    Texture* logo;

    glm::vec3 frontOffset;
    glm::vec3 bottomOffset;
    glm::vec3 roofOffset;
    glm::vec3 BackOffset;
    glm::vec3 steeringOffset;

    glm::vec3 BWOffset[3];
    glm::vec3 PedalOffset[2];
    glm::vec3 tailOffset[2];
    glm::vec3 headLightsOffset[2];
    glm::vec3 emblemsOffset[2];
    glm::vec3 sideMirrorOffset[2];
    glm::vec3 dividerOffsets[4];
    glm::vec3 divOffsets[2];
    glm::vec3 backdivOffsets[2];
    glm::vec3 wheelOffsets[4];
    glm::vec3 doorOffsets[4];
    glm::vec3 carseatsOffsets[4];
};
#endif



