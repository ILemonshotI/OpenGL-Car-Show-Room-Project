#include "ModelBoundingBox.h"
#include <limits>

// Default constructor
ModelBoundingBox::ModelBoundingBox()
    : initialized(false) {
}

// Constructor with explicit bounds
ModelBoundingBox::ModelBoundingBox(const glm::vec3& min, const glm::vec3& max)
    : localBounds(min, max), initialized(true) {
}

// Compute bounds from vertex list
void ModelBoundingBox::computeFromVertices(const std::vector<glm::vec3>& vertices) {
    if (vertices.empty()) {
        localBounds = BoundingBox(glm::vec3(0.0f), glm::vec3(0.0f));
        initialized = true;
        return;
    }

    // Initialize with first vertex
    glm::vec3 minVertex = vertices[0];
    glm::vec3 maxVertex = vertices[0];

    // Find min/max across all vertices
    for (size_t i = 1; i < vertices.size(); i++) {
        const glm::vec3& vertex = vertices[i];

        if (vertex.x < minVertex.x) minVertex.x = vertex.x;
        if (vertex.y < minVertex.y) minVertex.y = vertex.y;
        if (vertex.z < minVertex.z) minVertex.z = vertex.z;

        if (vertex.x > maxVertex.x) maxVertex.x = vertex.x;
        if (vertex.y > maxVertex.y) maxVertex.y = vertex.y;
        if (vertex.z > maxVertex.z) maxVertex.z = vertex.z;
    }

    localBounds = BoundingBox(minVertex, maxVertex);
    initialized = true;
}

// Get world-space bounds with transformation
BoundingBox ModelBoundingBox::getWorldBounds(const glm::mat4& modelMatrix) const {
    if (!initialized) {
        return BoundingBox(glm::vec3(0.0f), glm::vec3(0.0f));
    }
    return localBounds.getTransformed(modelMatrix);
}

// Static factory method for quick setup
ModelBoundingBox ModelBoundingBox::fromSize(const glm::vec3& size) {
    glm::vec3 halfSize = size * 0.5f;
    return ModelBoundingBox(-halfSize, halfSize);
}