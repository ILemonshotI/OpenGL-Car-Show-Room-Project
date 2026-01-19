//
//  SteeringWheel.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/16/26.
//

#include "SteeringWheel.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

SteeringWheel::SteeringWheel(
    Shader* shader,
    Texture* texture,
    glm::vec3 position,
    glm::vec3 rotationAxis,
    float rotationSpeed,
    float alpha
)
    : Object(shader, texture, position, rotationAxis, rotationSpeed, alpha)
{

    const int segments = 64;
    const float R = 0.12f;       // radius of the rim
    const float rimRadius = 0.018f;
    const float arcLen = (2.0f * glm::pi<float>() * R) / segments;

    for (int i = 0; i < segments; ++i)
    {
        float a = (2.0f * glm::pi<float>() / segments) * i;


        glm::vec3 pos(
            0.0f,              // X = 0
            cos(a) * R,
            sin(a) * R
        );

        float tangent = a + glm::half_pi<float>();

        Cylinder* seg = new Cylinder(
            rimRadius,
            rimRadius,
            arcLen,
            16,
            shader,
            texture,
            pos,
            glm::vec3(1, 0, 0),
            0.0f,
            alpha
        );

        seg->rotationAngle = glm::degrees(tangent);
        seg->useTexture = 1;
        rim.push_back(seg);
    }
    TOffsets[0] = glm::vec3(0.0f, 0.0f, 0.0f);
    centre = new Tcentre(shader, texture, TOffsets[0], glm::vec3(0, 1, 0), 0.0f, alpha);
    centre->useTexture = 1;

}

SteeringWheel::~SteeringWheel()
{
    for (auto* r : rim) delete r;

}

void SteeringWheel::drawWithParent(const glm::mat4& parent,
    const glm::mat4& view,
    const glm::mat4& proj)
{
    glm::mat4 model = parent;
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(1, 0, 0));

    // Draw rim
    for (auto* r : rim)
        r->drawWithParent(model, view, proj);
    centre->drawWithParent(model, view, proj);


}
