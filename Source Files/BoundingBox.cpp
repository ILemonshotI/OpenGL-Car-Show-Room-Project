#include "BoundingBox.h"
#include <algorithm>

// Default constructor
BoundingBox::BoundingBox() : min(0.0f, 0.0f, 0.0f), max(0.0f, 0.0f, 0.0f) {}

// Constructor with min/max
BoundingBox::BoundingBox(const glm::vec3& min, const glm::vec3& max)
    : min(min), max(max) {
}



// Get center of box
glm::vec3 BoundingBox::getCenter() const {
    return (min + max) * 0.5f;
}

// Get size of box
glm::vec3 BoundingBox::getSize() const {
    return max - min;
}

// Expand box to include point
void BoundingBox::expand(const glm::vec3& point) {
    min.x = std::min(min.x, point.x);
    min.y = std::min(min.y, point.y);
    min.z = std::min(min.z, point.z);

    max.x = std::max(max.x, point.x);
    max.y = std::max(max.y, point.y);
    max.z = std::max(max.z, point.z);
}

// Expand box to include another box
void BoundingBox::expand(const BoundingBox& other) {
    min.x = std::min(min.x, other.min.x);
    min.y = std::min(min.y, other.min.y);
    min.z = std::min(min.z, other.min.z);

    max.x = std::max(max.x, other.max.x);
    max.y = std::max(max.y, other.max.y);
    max.z = std::max(max.z, other.max.z);
}

// Transform box by model matrix - THIS IS THE KEY METHOD!
BoundingBox BoundingBox::getTransformed(const glm::mat4& transform) const {
    // Get all 8 corners of the box
    glm::vec3 corners[8] = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, max.y, max.z)
    };

    // Transform first corner
    glm::vec4 transformed = transform * glm::vec4(corners[0], 1.0f);
    glm::vec3 newMin = glm::vec3(transformed);
    glm::vec3 newMax = newMin;

    // Transform remaining corners and expand
    for (int i = 1; i < 8; i++) {
        transformed = transform * glm::vec4(corners[i], 1.0f);
        glm::vec3 point = glm::vec3(transformed);

        if (point.x < newMin.x) newMin.x = point.x;
        if (point.y < newMin.y) newMin.y = point.y;
        if (point.z < newMin.z) newMin.z = point.z;

        if (point.x > newMax.x) newMax.x = point.x;
        if (point.y > newMax.y) newMax.y = point.y;
        if (point.z > newMax.z) newMax.z = point.z;
    }

    return BoundingBox(newMin, newMax);
}

// Check intersection with another AABB
bool BoundingBox::intersects(const BoundingBox& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
        (min.y <= other.max.y && max.y >= other.min.y) &&
        (min.z <= other.max.z && max.z >= other.min.z);
}

// Check if point is inside box
bool BoundingBox::contains(const glm::vec3& point) const {
    return (point.x >= min.x && point.x <= max.x) &&
        (point.y >= min.y && point.y <= max.y) &&
        (point.z >= min.z && point.z <= max.z);
}

// Get vertices for debug rendering
std::vector<glm::vec3> BoundingBox::getVertices() const {
    std::vector<glm::vec3> vertices;
    vertices.reserve(8);

    vertices.push_back(glm::vec3(min.x, min.y, min.z));
    vertices.push_back(glm::vec3(max.x, min.y, min.z));
    vertices.push_back(glm::vec3(min.x, max.y, min.z));
    vertices.push_back(glm::vec3(max.x, max.y, min.z));
    vertices.push_back(glm::vec3(min.x, min.y, max.z));
    vertices.push_back(glm::vec3(max.x, min.y, max.z));
    vertices.push_back(glm::vec3(min.x, max.y, max.z));
    vertices.push_back(glm::vec3(max.x, max.y, max.z));

    return vertices;
}

// Get wireframe lines (12 lines, 24 vertices)
std::vector<glm::vec3> BoundingBox::getLines() const {
    std::vector<glm::vec3> lines;
    lines.reserve(24);

    auto vertices = getVertices();

    // Bottom square
    lines.push_back(vertices[0]); lines.push_back(vertices[1]);  // front edge
    lines.push_back(vertices[1]); lines.push_back(vertices[3]);  // right edge
    lines.push_back(vertices[3]); lines.push_back(vertices[2]);  // back edge
    lines.push_back(vertices[2]); lines.push_back(vertices[0]);  // left edge

    // Top square
    lines.push_back(vertices[4]); lines.push_back(vertices[5]);
    lines.push_back(vertices[5]); lines.push_back(vertices[7]);
    lines.push_back(vertices[7]); lines.push_back(vertices[6]);
    lines.push_back(vertices[6]); lines.push_back(vertices[4]);

    // Vertical edges
    lines.push_back(vertices[0]); lines.push_back(vertices[4]);  // front-left
    lines.push_back(vertices[1]); lines.push_back(vertices[5]);  // front-right
    lines.push_back(vertices[2]); lines.push_back(vertices[6]);  // back-left
    lines.push_back(vertices[3]); lines.push_back(vertices[7]);  // back-right

    return lines;
}