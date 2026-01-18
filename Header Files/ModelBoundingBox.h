// ModelBoundingBox.h
#pragma once
#include "BoundingBox.h"
#include <string>
#include <vector>

class ModelBoundingBox {
private:
    BoundingBox localBounds;
    bool initialized = false;

public:
    ModelBoundingBox();
    ModelBoundingBox(const glm::vec3& min, const glm::vec3& max);

    void computeFromVertices(const std::vector<glm::vec3>& vertices);

    // Optional: If you want to integrate with your model loader later
    // void computeFromModel(const YourModelClass& model);

    BoundingBox getWorldBounds(const glm::mat4& modelMatrix) const;
    const BoundingBox& getLocalBounds() const { return localBounds; }
    bool isInitialized() const { return initialized; }

    static ModelBoundingBox fromSize(const glm::vec3& size);
};