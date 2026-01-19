#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"
#include "building_Texture.h"
#include "building_VAO.h"
#include "building_VBO.h"
#include "building_EBO.h"
#include <vector>

using namespace glm;
using namespace std;

class buildingWall {
private:
    float width, height;
    vec3 position;
    vec3 rotation;
    buildingTexture wallTexture;
    buildingVAO wallVAO;
    buildingVBO wallVBO;
    buildingEBO wallEBO;
    bool hasTexture;
    bool inverted;
    void setupGeometry();
    vec2 textureScale;

public:

    buildingWall() : width(0), height(0), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f),
        hasTexture(false), inverted(false) {
    }

    buildingWall(float width, float height, const char* texturePath,
        vec3 pos, vec3 rot, bool isInverted = false);

    buildingWall(float width, float height, vec3 pos, vec3 rot, bool isInverted = false);


    void Draw(Shader& shader, mat4 view, mat4 projection);
    void DrawWithTransform(Shader& shader, mat4 parentTransform, mat4 view, mat4 projection);
    void setPosition(vec3 newPos);
    void setRotation(vec3 newRot);
    void setScale(float newWidth, float newHeight);
    void move(vec3 offset);
    void rotate(vec3 angles);
    void setTextureScale(float scaleX, float scaleY);


    vec3 getPosition() const { return position; }
    vec3 getRotation() const { return rotation; }

    void Delete();
};