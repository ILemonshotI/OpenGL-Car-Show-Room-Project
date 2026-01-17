//
//  object.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/9/26.
//

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glm/glm.hpp>
#include "VAO.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Material.hpp"

class Object
{
public:
    Object(Shader* shader = nullptr, Texture* texture = nullptr,
           glm::vec3 position = glm::vec3(0.0f),
           glm::vec3 rotationAxis = glm::vec3(0,1,0),
           float rotationSpeed = 0.0f,
           float alpha = 1.0f);
    
    Object(Shader* shader = nullptr,
           glm::vec3 position = glm::vec3(0.0f),
           glm::vec3 rotationAxis = glm::vec3(0,1,0),
           float rotationSpeed = 0.0f,
           float alpha = 1.0f);

    virtual ~Object() {}

    virtual void update(float dt);

    // NEW
    virtual glm::mat4 getModelMatrix() const;

    virtual void draw(const glm::mat4& view, const glm::mat4& proj);
    virtual void drawWithParent(const glm::mat4& parent,
                                const glm::mat4& view,
                                const glm::mat4& proj);

    glm::vec3 getPosition() const { return position; }


public:
    Shader* shader;
    Texture* texture;
    VAO* vao = nullptr;

    glm::vec3 position;
    glm::vec3 rotationAxis;
    float rotationSpeed;
    float rotationAngle;
    glm::vec4 color = glm::vec4(1);
    bool useTexture = true;
    float alpha;
};
#endif
