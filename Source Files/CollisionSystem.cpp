#include "CollisionSystem.h"
#include <algorithm>

// Constructor with world bounds
CollisionSystem::CollisionSystem(const BoundingBox& worldBounds)
    : worldBounds(worldBounds) {
}

// Register a collidable object
void CollisionSystem::registerCollidable(std::shared_ptr<Collidable> collidable) {
    if (collidable) {
        collidables.push_back(collidable);
    }
}

// Unregister a collidable object
void CollisionSystem::unregisterCollidable(std::shared_ptr<Collidable> collidable) {
    if (!collidable) return;

    auto it = std::find(collidables.begin(), collidables.end(), collidable);
    if (it != collidables.end()) {
        collidables.erase(it);
    }
}

// Check collision with any object
bool CollisionSystem::checkCollision(const BoundingBox& box) const {
    // Check world boundaries first
    if (!worldBounds.intersects(box)) {
        return true; // Collision with world boundary
    }

    // Check against all collidables
    for (const auto& collidable : collidables) {
        if (collidable && collidable->getBoundingBox().intersects(box)) {
            return true;
        }
    }

    return false;
}

// Get all colliding objects
std::vector<std::shared_ptr<Collidable>> CollisionSystem::getCollisions(const BoundingBox& box) const {
    std::vector<std::shared_ptr<Collidable>> collisions;

    // Only check if inside world bounds
    if (!worldBounds.intersects(box)) {
        return collisions; // Empty - treat world bounds separately
    }

    for (const auto& collidable : collidables) {
        if (collidable && collidable->getBoundingBox().intersects(box)) {
            collisions.push_back(collidable);
        }
    }

    return collisions;
}

// Check if camera can move to position
bool CollisionSystem::canMoveTo(const glm::vec3& position, const glm::vec3& cameraSize) const {
    BoundingBox cameraBox(
        position - cameraSize * 0.5f,
        position + cameraSize * 0.5f
    );
    return !checkCollision(cameraBox);
}

// Update system (for dynamic objects - placeholder)
void CollisionSystem::update() {
    // Currently static, but could handle:
    // - Moving objects
    // - Updating spatial partitioning
    // - Handling collision callbacks
}

// Set new world bounds
void CollisionSystem::setWorldBounds(const BoundingBox& bounds) {
    worldBounds = bounds;
}

// Clear all collidables
void CollisionSystem::clear() {
    collidables.clear();
}

// Get collidable count
size_t CollisionSystem::getCollidableCount() const {
    return collidables.size();
}