#ifndef GROUND_CLASS_H
#define GROUND_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "building_VAO.h"
#include "building_VBO.h"
#include "building_EBO.h"
#include "Shader.hpp"
#include "building_Texture.h"

using namespace glm;

class buildingGround {
public:
    buildingVAO groundVAO;
    buildingVBO groundVBO;
    buildingEBO groundEBO;
    buildingTexture groundTexture;
    float size;
    bool hasTexture;

    buildingGround(float size = 50.0f, const char* texturePath = nullptr);

    void Draw(Shader& shader, mat4 view, mat4 projection);
    void Delete();
    void setupGround(float size);
};

#endif