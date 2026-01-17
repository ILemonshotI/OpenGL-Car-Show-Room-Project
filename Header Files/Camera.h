// Camera.h
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>

#include "Shader.hpp"

using namespace glm;

class Camera {
public:
    // Collision structure
    struct BoundingBox {
        vec3 min;
        vec3 max;

        BoundingBox(const vec3& minBounds = vec3(-1.0f), const vec3& maxBounds = vec3(1.0f))
            : min(minBounds), max(maxBounds) {
        }
    };

    vec3 Position;
    vec3 Orientation = vec3(0.0f, 0.0f, -1.0f);
    vec3 Up = vec3(0.0f, 1.0f, 0.0f);
    mat4 cameraMatrix = mat4(1.0f);

    // Camera dimensions for collision
    vec3 cameraSize = vec3(0.5f, 1.0f, 0.5f); // Width, Height, Depth

    bool firstClick = true;

    int width;
    int height;

    float speed = 0.01f;
    float sensitivity = 100.0f;

    // Collision boundaries
    BoundingBox worldBounds;
    std::vector<BoundingBox> obstacles;

    Camera(int width, int height, vec3 position);
    float distanceTo(const glm::vec3& point) const;

    // Collision methods
    bool checkCollision(const BoundingBox& a, const BoundingBox& b) const;
    BoundingBox getCameraBoundingBox() const;
    bool canMoveTo(const vec3& newPosition) const;
    void addObstacle(const BoundingBox& obstacle);
    void setWorldBounds(const BoundingBox& bounds);

    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void Matrix(Shader& shader, const char* uniform);
    void Inputs(GLFWwindow* window);
};

#endif