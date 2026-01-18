// Collidable.h
#pragma once
#include "BoundingBox.h"
#include <glm/glm.hpp>

class Collidable {
public:
    virtual ~Collidable() = default;

    // Get bounding box in world space
    virtual BoundingBox getBoundingBox() const = 0;

    // Optional: get transform for dynamic updates
    virtual glm::mat4 getModelMatrix() const { return glm::mat4(1.0f); }

    // For static vs dynamic objects
    virtual bool isStatic() const { return true; }

    // Callback for collision events
    virtual void onCollision(const Collidable* other) {}

    // For debugging
    virtual const char* getName() const { return "Collidable"; }
};