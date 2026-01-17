// Camera.cpp
#include "Camera.h"
#include <iostream>

Camera::Camera(int width, int height, vec3 position) {
    Camera::width = width;
    Camera::height = height;
    Position = position;

    // Default world bounds (you can customize these)
    worldBounds = BoundingBox(vec3(-10.0f, 0.0f, -10.0f), vec3(10.0f, 10.0f, 10.0f));
}

// Get camera's bounding box at current position
Camera::BoundingBox Camera::getCameraBoundingBox() const {
    BoundingBox box;
    box.min = Position - cameraSize * 0.5f;
    box.max = Position + cameraSize * 0.5f;
    return box;
}

// Check collision between two AABBs (Axis-Aligned Bounding Boxes)
bool Camera::checkCollision(const BoundingBox& a, const BoundingBox& b) const {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
        (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
        (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

// Check if camera can move to new position without colliding
bool Camera::canMoveTo(const vec3& newPosition) const {
    // Create temporary bounding box for the new position
    BoundingBox newBox;
    newBox.min = newPosition - cameraSize * 0.5f;
    newBox.max = newPosition + cameraSize * 0.5f;

    // Check world bounds
    if (!checkCollision(newBox, worldBounds)) {
        return false;
    }

    // Check obstacles
    for (const auto& obstacle : obstacles) {
        if (checkCollision(newBox, obstacle)) {
            return false;
        }
    }

    return true;
}

// Add an obstacle to the world
void Camera::addObstacle(const BoundingBox& obstacle) {
    obstacles.push_back(obstacle);
}

// Set world boundaries
void Camera::setWorldBounds(const BoundingBox& bounds) {
    worldBounds = bounds;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
    mat4 view = mat4(1.0f);
    mat4 projection = mat4(1.0f);
    view = lookAt(Position, Position + Orientation, Up);
    projection = perspective(radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

    cameraMatrix = projection * view;
}

float Camera::distanceTo(const glm::vec3& point) const {
    return glm::length(Position - point);
}

void Camera::Matrix(Shader& shader, const char* uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window) {
    vec3 newPosition = Position;

    // Movement with collision detection
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 tempPos = Position + speed * Orientation;
        if (canMoveTo(tempPos)) {
            newPosition = tempPos;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 tempPos = Position + speed * -normalize(cross(Orientation, Up));
        if (canMoveTo(tempPos)) {
            newPosition = tempPos;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 tempPos = Position + speed * -Orientation;
        if (canMoveTo(tempPos)) {
            newPosition = tempPos;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 tempPos = Position + speed * normalize(cross(Orientation, Up));
        if (canMoveTo(tempPos)) {
            newPosition = tempPos;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        vec3 tempPos = Position + speed * Up;
        if (canMoveTo(tempPos)) {
            newPosition = tempPos;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        vec3 tempPos = Position + speed * -Up;
        if (canMoveTo(tempPos)) {
            newPosition = tempPos;
        }
    }

    // Update position if no collision
    Position = newPosition;

    // Sprint
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed = 0.04f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        speed = 0.01f;
    }

    // Mouse look
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick) {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (height / 2)) / height;

        vec3 newOrientation = rotate(Orientation, radians(-rotX), normalize(cross(Orientation, Up)));

        if (!(angle(newOrientation, Up) <= radians(5.0f) or angle(newOrientation, -Up) <= radians(5.0f))) {
            Orientation = newOrientation;
        }

        Orientation = rotate(Orientation, radians(-rotY), Up);
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}