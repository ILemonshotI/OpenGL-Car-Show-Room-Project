//
//  SlidingDoor.h
//  OpenGLProject
//
//  Created by Ameen on 1/18/26.
//

#ifndef SLIDINGDOOR_HPP
#define SLIDINGDOOR_HPP

#include "Object.hpp"
#include "Camera.h"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class SlidingDoor : public Object
{
public:
    SlidingDoor(
        Shader* shader,
        Texture* texture,
        glm::vec3 position,
        float doorWidth,
        float doorHeight,
        float doorDepth,
        float triggerDistance = 5.0f,
        float slideSpeed = 4.0f,
        float alpha = 1.0f
    );

    ~SlidingDoor();

    void update(float dt, const Camera& camera);
    void draw(const glm::mat4& view, const glm::mat4& proj);

private:
    // Left panel
    VAO leftVAO;
    VBO* leftVBO;
    EBO* leftEBO;

    // Right panel
    VAO rightVAO;
    VBO* rightVBO;
    EBO* rightEBO;

    glm::vec3 leftPos;
    glm::vec3 rightPos;

    glm::vec3 leftClosed;
    glm::vec3 rightClosed;
    glm::vec3 leftOpen;
    glm::vec3 rightOpen;

    float activationDistance;
    float speed;
    bool isOpen = false;

    void setupPanelMesh(
        VAO& vao,
        VBO*& vbo,
        EBO*& ebo,
        float width,
        float height,
        float depth
    );
};

#endif
