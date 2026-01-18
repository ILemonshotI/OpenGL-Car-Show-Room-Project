// CollisionSystem.h
#pragma once
#include "Collidable.h"
#include <vector>
#include <memory>

class CollisionSystem {
private:
    std::vector<std::shared_ptr<Collidable>> collidables;
    BoundingBox worldBounds;

public:
    CollisionSystem(const BoundingBox& worldBounds);

    void registerCollidable(std::shared_ptr<Collidable> collidable);
    void unregisterCollidable(std::shared_ptr<Collidable> collidable);

    bool checkCollision(const BoundingBox& box) const;
    std::vector<std::shared_ptr<Collidable>> getCollisions(const BoundingBox& box) const;

    bool canMoveTo(const glm::vec3& position, const glm::vec3& cameraSize) const;

    void update();
    void setWorldBounds(const BoundingBox& bounds);
    void clear();

    const std::vector<std::shared_ptr<Collidable>>& getAllCollidables() const {
        return collidables;
    }

    size_t getCollidableCount() const;
};