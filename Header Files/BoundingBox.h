// BoundingBox.h
#pragma once
#include <glm/glm.hpp>
#include <vector>

class BoundingBox {
public:
    glm::vec3 min;
    glm::vec3 max;

    // Constructors
    BoundingBox();
    BoundingBox(const glm::vec3& min, const glm::vec3& max);

    // Utility methods
    glm::vec3 getCenter() const;
    glm::vec3 getSize() const;
    void expand(const glm::vec3& point);
    void expand(const BoundingBox& other);

    // Transform support
    BoundingBox getTransformed(const glm::mat4& transform) const;

    // Collision detection
    bool intersects(const BoundingBox& other) const;
    bool contains(const glm::vec3& point) const;

    // Debug visualization
    std::vector<glm::vec3> getVertices() const;
    std::vector<glm::vec3> getLines() const;
};