#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f); // Euler angles in degrees: X(pitch), Y(yaw), Z(roll)
    glm::vec3 scale = glm::vec3(1.0f);

    Transform() = default;
    Transform(glm::vec3 pos, glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scl = glm::vec3(1.0f))
        : position(pos), rotation(rot), scale(scl) {
    }

    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);

        // Order: Translate -> Rotate -> Scale
        model = glm::translate(model, position);

        // Rotate around Y, X, Z (Yaw, Pitch, Roll)
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0)); // Yaw
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0)); // Pitch  
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1)); // Roll

        model = glm::scale(model, scale);

        return model;
    }
};