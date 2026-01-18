//
//  object.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/9/26.
//

#include "Object.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Object::Object(Shader* shader, Texture* texture,
    glm::vec3 position,
    glm::vec3 rotationAxis,
    float rotationSpeed,
    float alpha)
    : shader(shader), texture(texture),
    position(position), rotationAxis(rotationAxis),
    rotationSpeed(rotationSpeed), rotationAngle(0.0f),
    alpha(alpha) {
}

Object::Object(Shader* shader,
    glm::vec3 position,
    glm::vec3 rotationAxis,
    float rotationSpeed,
    float alpha)
    : shader(shader),
    position(position), rotationAxis(rotationAxis),
    rotationSpeed(rotationSpeed), rotationAngle(0.0f),
    alpha(alpha)
{
}


void Object::update(float deltaTime)
{
    rotationAngle += rotationSpeed * deltaTime;
    if (rotationAngle > 360.0f) rotationAngle -= 360.0f;
}

glm::mat4 Object::getModelMatrix() const
{
    glm::mat4 model(1.0f);

    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
    model = glm::scale(model, scale);   

    return model;
}


void Object::draw(const glm::mat4& view, const glm::mat4& proj)
{
    drawWithParent(glm::mat4(1.0f), view, proj);
}

void Object::drawWithParent(const glm::mat4& parent,
    const glm::mat4& view,
    const glm::mat4& proj)
{
    if (!vao || !shader) return;

    shader->Activate();
    glm::mat4 model = parent * getModelMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "proj"), 1, GL_FALSE, &proj[0][0]);

    glm::vec4 c = color; c.a = alpha;
    glUniform4fv(glGetUniformLocation(shader->ID, "objectColor"), 1, &c[0]);
    glUniform1i(glGetUniformLocation(shader->ID, "useTexture"), useTexture);

    if (useTexture && texture)
        texture->Bind();

    vao->Bind();
    glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);
    vao->Unbind();
}
void Object::translate(const glm::vec3& delta)
{
    position += delta;
}

void Object::setPosition(const glm::vec3& pos)
{
    position = pos;
}

void Object::rotate(float degrees)
{
    rotationAngle += degrees;
}

void Object::setRotation(float degrees)
{
    rotationAngle = degrees;
}

void Object::setScale(const glm::vec3& s)
{
    scale = s;
}

void Object::scaleBy(const glm::vec3& factor)
{
    scale *= factor;
}

Camera::BoundingBox Object::getBoundingBox() const
{
    glm::vec3 margin = glm::vec3(0.15f);
    glm::vec3 half = baseSize * scale * 0.5f + margin;

    glm::vec3 localMin = -half;
    glm::vec3 localMax = half;

    glm::mat4 model = getModelMatrix();

    glm::vec3 corners[8] = {
        {localMin.x, localMin.y, localMin.z},
        {localMax.x, localMin.y, localMin.z},
        {localMin.x, localMax.y, localMin.z},
        {localMax.x, localMax.y, localMin.z},
        {localMin.x, localMin.y, localMax.z},
        {localMax.x, localMin.y, localMax.z},
        {localMin.x, localMax.y, localMax.z},
        {localMax.x, localMax.y, localMax.z}
    };

    glm::vec3 worldMin(FLT_MAX), worldMax(-FLT_MAX);

    for (auto& c : corners) {
        glm::vec3 w = glm::vec3(model * glm::vec4(c, 1.0));
        worldMin = glm::min(worldMin, w);
        worldMax = glm::max(worldMax, w);
    }

    return { worldMin, worldMax };
}

