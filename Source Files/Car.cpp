//
//  car.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/9/26.
//
#include "Car.hpp"
#include <glm/gtc/matrix_transform.hpp>



Car::Car(Shader* shader, Texture* frontTexture, Texture* wheelTexture,
         glm::vec3 position,
         glm::vec3 rotationAxis,
         float rotationSpeed,
         float alpha)
    : Object(shader, frontTexture, position, rotationAxis, rotationSpeed, alpha),
      wheelTexture(wheelTexture),
      frontTexture(frontTexture)
{
    // ---------- LOCAL OFFSETS ----------
    frontOffset   = glm::vec3(-1.1f,  0.0f,  0.0f);
    bottomOffset  = glm::vec3( 0.0f, -0.3f,  0.0f);
    roofOffset    = glm::vec3( 0.0f,  0.6f,  0.0f);
    BackOffset    = glm::vec3( 0.8f,  0.0f,  0.0f);

    BWOffset[0] = glm::vec3(-0.8f, 0.3f, 0.0f);
    BWOffset[1] = glm::vec3( 0.8f, 0.3f, 0.0f);

    dividerOffsets[0] = glm::vec3( 0.4f,-0.1f, 0.6f);//small div
    dividerOffsets[1] = glm::vec3( 0.4f,-0.1f,-0.6f);
    dividerOffsets[2] = glm::vec3(-0.9f,-0.1f, 0.6f);
    dividerOffsets[3] = glm::vec3(-0.9f,-0.1f,-0.6f);//
    
    sideMirrorOffset[0] = glm::vec3(-0.83f,0.3f, 0.65f);
    sideMirrorOffset[1] = glm::vec3(-0.83f,0.3f,-0.75f);
    
    tailOffset[0] = glm::vec3(0.81f,-0.1f, 0.3f);
    tailOffset[1] = glm::vec3(0.81f,-0.1f,-0.55f);
    
    headLightsOffset[0] = glm::vec3(-1.15f,-0.13f, 0.46f);
    headLightsOffset[1] = glm::vec3(-1.15f,-0.13f,-0.55f);

    divOffsets[0] = glm::vec3(-0.3f,-0.2f, 0.6f);//middle div
    divOffsets[1] = glm::vec3(-0.3f,-0.2f,-0.6f);//
    
    backdivOffsets[0] = glm::vec3(0.6f,-0.2f, 0.6f);
    backdivOffsets[1] = glm::vec3(0.6f,-0.2f,-0.6f);


    wheelOffsets[0] = glm::vec3(-0.8f,-0.2f, 0.6f);
    wheelOffsets[1] = glm::vec3( 0.45f,-0.2f, 0.6f);
    wheelOffsets[2] = glm::vec3(-0.8f,-0.2f,-0.6f);
    wheelOffsets[3] = glm::vec3( 0.45f,-0.2f,-0.6f);

    doorOffsets[0] = glm::vec3(-0.45f,0.0f, 0.6f);
    doorOffsets[1] = glm::vec3( 0.45f,0.0f, 0.6f);
    doorOffsets[2] = glm::vec3(-0.45f,0.0f,-0.6f);
    doorOffsets[3] = glm::vec3( 0.45f,0.0f,-0.6f);
    
    


    // ---------- CREATE PARTS (LOCAL SPACE) ----------
    front  = new CarFront(shader, frontTexture, frontOffset,  glm::vec3(0,1,0), 0.0f, alpha);
    front->color= glm::vec4(0.22f,1.0f,0.08f,1.0f);
    front->useTexture=1;
    back   = new CarBack(shader,  frontTexture, BackOffset,   glm::vec3(0,1,0), 0.0f, alpha);
    back->color= glm::vec4(0.22f,1.0f,0.08f,1.0f);
    back->useTexture=1;
    bottom = new CarBottom(shader,frontTexture, bottomOffset, glm::vec3(0,1,0), 0.0f, alpha);
    bottom->color= glm::vec4(0.22f,1.0f,0.08f,1.0f);
    bottom->useTexture=1;
    roof   = new CarRoof(shader,  frontTexture, roofOffset,   glm::vec3(0,1,0), 0.0f, alpha);
    roof->color= glm::vec4(0.22f,1.0f,0.08f,1.0f);
    roof->useTexture=1;

    for(int i=0;i<2;i++)
    {
        window[i] = new BackWindow(shader,
                                   BWOffset[i],
                                   glm::vec3(0,1,0),
                                   0.0f,
                                   alpha);
        window[i]-> useTexture=false;
        
        TailLights[i] = new tailLights(shader,texture,
                                   tailOffset[i],
                                   glm::vec3(0,1,0),
                                   0.0f,
                                   alpha);
        TailLights[i]-> useTexture=false;
        TailLights[i]->color= glm::vec4(1.0f,0.0f,0.0f,0.6f);
        
        headLights[i] = new HeadLights(shader,texture,
                                   headLightsOffset[i],
                                   glm::vec3(0,1,0),
                                   0.0f,
                                   alpha);
        headLights[i]-> useTexture=false;
        headLights[i]->color= glm::vec4(1.0f,1.0f,0.0f,0.6f);

        div[i] = new MidDiv(shader,
                            frontTexture,
                            divOffsets[i],
                            glm::vec3(0,1,0),
                            0.0f,
                            alpha);
        div[i]->color= glm::vec4(0.22f,1.0f,0.08f,1.0f);
        div[i]->useTexture=1;
        
        sideMirror[i] = new SideMirror(shader,
                            frontTexture,
                            sideMirrorOffset[i],
                            glm::vec3(0,1,0),
                            0.0f,
                            alpha);
        sideMirror[i]->color= glm::vec4(0.22f,1.0f,0.08f,1.0f);
        sideMirror[i]->useTexture=1;
        
        backdiv[i] = new BackDiv(shader,
                            frontTexture,
                            backdivOffsets[i],
                            glm::vec3(0,1,0),
                            0.0f,
                            alpha);
        backdiv[i]->color= glm::vec4(0.22f,1.0f,0.08f,1.0f);
        backdiv[i]->useTexture=1;
    }

    for(int i=0;i<4;i++)
    {
        divider[i] = new WDiv(shader,
                              frontTexture,
                              dividerOffsets[i],
                              glm::vec3(0,1,0),
                              0.0f,
                              alpha);
        divider[i]->useTexture=1;
        divider[i]->color= glm::vec4(0.22f,1.0f,0.08f,1.0f);

        wheels[i] = new Cylinder(
            0.2f, 0.2f, 0.3f, 36,
            shader,
            wheelTexture,
            wheelOffsets[i],
            glm::vec3(0,1,0),
            120.0f,
            alpha
            
        );
        wheels[i] -> useTexture=true;
        CarDoors[i] = new CarDoor(shader,
                                  frontTexture,
                                  doorOffsets[i],
                                  glm::vec3(0,1,0),
                                  0.0f);
    }
}

void Car::ToggleDriverDoor(const glm::vec3& cameraPos)
{
    glm::vec3 carWorldPos = position; // Object::position
    float distance = glm::length(cameraPos - carWorldPos);

    if (distance < 5.0f) // interaction radius
    {
        CarDoors[0]->toggle();
    }
}

Car::~Car()
{
    delete front;
    delete back;
    delete bottom;
    delete roof;

    for(int i=0;i<4;i++)
    {
        delete wheels[i];
        delete CarDoors[i];
        delete divider[i];
    }

    for(int i=0;i<2;i++)
    {
        delete window[i];
        delete div[i];
    }
}

void Car::update(float deltaTime)
{
    // Car moves / rotates as ONE UNIT
    Object::update(deltaTime);

    // Wheels spin locally
    for(int i=0;i<4;i++)
        wheels[i]->update(deltaTime);
    CarDoors[0]-> update(deltaTime);
}

void Car::draw(const glm::mat4& view, const glm::mat4& proj)
{
    glm::mat4 carModel = getModelMatrix();
    
    // -------- BODY --------
    Material::setPBR(*shader, 1.0f, 0.2f, glm::vec3(0.22f,1.0f,0.08f));
    
    front->drawWithParent(carModel, view, proj);
    back->drawWithParent(carModel, view, proj);
    bottom->drawWithParent(carModel, view, proj);
    roof->drawWithParent(carModel, view, proj);
    
    for(int i=0;i<4;i++)
        divider[i]->drawWithParent(carModel, view, proj);
    
    for(int i=0;i<2;i++)
    { div[i]->drawWithParent(carModel, view, proj);
        backdiv[i]->drawWithParent(carModel, view, proj);
        sideMirror[i]->drawWithParent(carModel, view, proj);
        sideMirror[i]->drawWithParent(carModel, view, proj);
    }

    Material::unSetPBR(*shader);
    
    for(int i=0;i<2;i++)
    {
        TailLights[i]->drawWithParent(carModel, view, proj);
        headLights[i]->drawWithParent(carModel, view, proj);}

    // -------- WHEELS --------
    for(int i=0;i<4;i++)
        wheels[i]->drawWithParent(carModel, view, proj);

    // -------- DOORS --------
    for(int i=0;i<4;i++)
        CarDoors[i]->drawWithParent(carModel, view, proj);

    // -------- GLASS --------
    Material::setGlass(*shader, 4.0f, 0.15f);
    for(int i=0;i<2;i++)
    {window[i]->drawWithParent(carModel, view, proj);
        TailLights[i]->drawWithParent(carModel, view, proj);}
    Material::unSetGlass(*shader);
  
}

