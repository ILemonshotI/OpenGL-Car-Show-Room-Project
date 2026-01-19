//
//  SlidingDoor.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/18/26.
//

#include "SlidingDoor.hpp"

SlidingDoor::SlidingDoor(
    Shader* shader,
    Texture* texture,
    glm::vec3 position,
    float doorWidth,
    float doorHeight,
    float doorDepth,
    float triggerDistance,
    float slideSpeed,
    float alpha
)
    : Object(shader, texture, position, glm::vec3(0, 1, 0), 0.0f, alpha),
    activationDistance(triggerDistance),
    speed(slideSpeed)
{
    setupPanelMesh(leftVAO, leftVBO, leftEBO,
        doorWidth * 0.5f, doorHeight, doorDepth);

    setupPanelMesh(rightVAO, rightVBO, rightEBO,
        doorWidth * 0.5f, doorHeight, doorDepth);

    leftClosed = position + glm::vec3(-doorWidth * 0.25f, 0.0f, 0.0f);
    rightClosed = position + glm::vec3(doorWidth * 0.25f, 0.0f, 0.0f);

    leftOpen = leftClosed + glm::vec3(-doorWidth * 0.5f, 0.0f, 0.0f);
    rightOpen = rightClosed + glm::vec3(doorWidth * 0.5f, 0.0f, 0.0f);

    leftPos = leftClosed;
    rightPos = rightClosed;
}

SlidingDoor::~SlidingDoor()
{
    leftVAO.Delete();
    rightVAO.Delete();

    leftVBO->Delete();
    rightVBO->Delete();

    leftEBO->Delete();
    rightEBO->Delete();
}

void SlidingDoor::update(float dt, const Camera& camera)
{
    float distance = camera.distanceTo(position);
    isOpen = distance < activationDistance;

    glm::vec3 leftTarget = isOpen ? leftOpen : leftClosed;
    glm::vec3 rightTarget = isOpen ? rightOpen : rightClosed;

    leftPos = glm::mix(leftPos, leftTarget, speed * dt);
    rightPos = glm::mix(rightPos, rightTarget, speed * dt);
}

void SlidingDoor::draw(const glm::mat4& view, const glm::mat4& proj)
{
    // Draw left panel
    Material::setGlass(*shader, 4.0f, 0.15f);
    glEnable(GL_BLEND);                                  // Enable mixing colors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    // Standard transparency formula
    glDepthMask(GL_FALSE);

    vao = &leftVAO;
    setPosition(leftPos);
    Object::draw(view, proj);

    // Draw right panel
    vao = &rightVAO;
    setPosition(rightPos);
    Object::draw(view, proj);

    glDepthMask(GL_TRUE);   // Re-enable depth writing for other objects
    glDisable(GL_BLEND);
    Material::unSetGlass(*shader);
}

void SlidingDoor::setupPanelMesh(
    VAO& vao,
    VBO*& vbo,
    EBO*& ebo,
    float width,
    float height,
    float depth)
{
    float x0 = 0.0f;
    float x1 = width;
    float y0 = 0.0f;
    float y1 = height;
    float z0 = -depth * 0.5f;
    float z1 = depth * 0.5f;

    GLfloat vertices[] = {
        // FRONT
        x0,y0,z1,  0,0,1,  0,0,
        x1,y0,z1,  0,0,1,  1,0,
        x1,y1,z1,  0,0,1,  1,1,
        x0,y1,z1,  0,0,1,  0,1,

        // RIGHT
        x1,y0,z1,  1,0,0,  0,0,
        x1,y0,z0,  1,0,0,  1,0,
        x1,y1,z0,  1,0,0,  1,1,
        x1,y1,z1,  1,0,0,  0,1,

        // BACK
        x1,y0,z0,  0,0,-1, 0,0,
        x0,y0,z0,  0,0,-1, 1,0,
        x0,y1,z0,  0,0,-1, 1,1,
        x1,y1,z0,  0,0,-1, 0,1,

        // LEFT
        x0,y0,z0, -1,0,0,  0,0,
        x0,y0,z1, -1,0,0,  1,0,
        x0,y1,z1, -1,0,0,  1,1,
        x0,y1,z0, -1,0,0,  0,1,

        // TOP
        x0,y1,z1,  0,1,0,  0,0,
        x1,y1,z1,  0,1,0,  1,0,
        x1,y1,z0,  0,1,0,  1,1,
        x0,y1,z0,  0,1,0,  0,1,

        // BOTTOM
        x0,y0,z0,  0,-1,0, 0,0,
        x1,y0,z0,  0,-1,0, 1,0,
        x1,y0,z1,  0,-1,0, 1,1,
        x0,y0,z1,  0,-1,0, 0,1
    };

    GLuint indices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };

    vao.Bind();

    vbo = new VBO(vertices, sizeof(vertices));
    ebo = new EBO(indices, sizeof(indices));

    vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    vao.Unbind();
}
