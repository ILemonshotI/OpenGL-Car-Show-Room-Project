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
alpha(alpha){}

Object::Object(Shader* shader,
               glm::vec3 position,
               glm::vec3 rotationAxis,
               float rotationSpeed,
               float alpha)
    : shader(shader),
      position(position), rotationAxis(rotationAxis),
      rotationSpeed(rotationSpeed), rotationAngle(0.0f),
alpha(alpha)
{}


void Object::update(float deltaTime)
{
    rotationAngle += rotationSpeed * deltaTime;
    if(rotationAngle > 360.0f) rotationAngle -= 360.0f;
}

glm::mat4 Object::getModelMatrix() const
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
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
    if(!vao || !shader) return;

    shader->Activate();
    glm::mat4 model = parent * getModelMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shader->ID,"model"),1,GL_FALSE,&model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID,"view"),1,GL_FALSE,&view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID,"proj"),1,GL_FALSE,&proj[0][0]);

    glm::vec4 c = color; c.a = alpha;
    glUniform4fv(glGetUniformLocation(shader->ID,"objectColor"),1,&c[0]);
    glUniform1i(glGetUniformLocation(shader->ID,"useTexture"), useTexture);

    if(useTexture && texture)
        texture->Bind();

    vao->Bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    vao->Unbind();
}
