#include "building_Wall.h"
#include <iostream>

using namespace glm;
using namespace std;

buildingWall::buildingWall(float width, float height, const char* texturePath,
    vec3 pos, vec3 rot, bool isInverted)
    : width(width), height(height), position(pos), rotation(rot),
    hasTexture(texturePath != nullptr), inverted(isInverted), textureScale(vec2(1.0f, 1.0f))
{
    setupGeometry();

    if (hasTexture) {
        wallTexture = buildingTexture(texturePath, GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
        wallTexture.Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        wallTexture.Unbind();
    }
}

buildingWall::buildingWall(float width, float height, vec3 pos, vec3 rot, bool isInverted)
    : width(width), height(height), position(pos), rotation(rot),
    hasTexture(false), inverted(isInverted), textureScale(vec2(1.0f, 1.0f))
{
    setupGeometry();
}

void buildingWall::setupGeometry() {
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    // Set normals based on inverted flag
    float normalZ = inverted ? -1.0f : 1.0f;

    float vertices[] = {
        // Positions          // Texture Coords  // Normals (facing +Z by default, -Z if inverted)
        -halfWidth, -halfHeight, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, normalZ,  // Bottom-left
         halfWidth, -halfHeight, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, normalZ,  // Bottom-right
         halfWidth,  halfHeight, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, normalZ,  // Top-right
        -halfWidth,  halfHeight, 0.0f,   0.0f, 1.0f,   0.0f, 0.0f, normalZ   // Top-left
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    wallVAO.Bind();
    wallVBO = buildingVBO(vertices, sizeof(vertices));
    wallEBO = buildingEBO(indices, sizeof(indices));

    wallVAO.LinkAttrib(wallVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    wallVAO.LinkAttrib(wallVBO, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    wallVAO.LinkAttrib(wallVBO, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    wallVAO.Unbind();
    wallVBO.Unbind();
    wallEBO.Unbind();
}

// Rest of the functions remain the same...
void buildingWall::Draw(Shader& shader, mat4 view, mat4 projection) {
    shader.Activate();

    mat4 model = mat4(1.0f);
    model = translate(model, position);

    model = glm::rotate(model, radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));

    model = scale(model, vec3(1.0f, 1.0f, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, value_ptr(projection));

    GLuint tilingLoc = glGetUniformLocation(shader.ID, "tiling");
    if (tilingLoc != -1) {
        float tilingX = (width / 2.0f) / textureScale.x;
        float tilingY = (height / 2.0f) / textureScale.y;
        glUniform2f(tilingLoc, tilingX, tilingY);
    }

    GLuint hasTextureLoc = glGetUniformLocation(shader.ID, "hasTexture");
    if (hasTextureLoc != -1) {
        glUniform1i(hasTextureLoc, hasTexture);
    }

    if (hasTexture) {
        wallTexture.Bind();
        wallTexture.texUnit(shader, "wallTexture", 0);
    }

    wallVAO.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    wallVAO.Unbind();

    if (hasTexture) {
        wallTexture.Unbind();
    }
}

void buildingWall::DrawWithTransform(Shader& shader, mat4 parentTransform, mat4 view, mat4 projection) {
    shader.Activate();

    mat4 localModel = mat4(1.0f);
    localModel = translate(localModel, position);
    localModel = glm::rotate(localModel, radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
    localModel = glm::rotate(localModel, radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
    localModel = glm::rotate(localModel, radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));

    mat4 finalModel = parentTransform * localModel;

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, value_ptr(finalModel));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, value_ptr(projection));

    GLuint tilingLoc = glGetUniformLocation(shader.ID, "tiling");
    if (tilingLoc != -1) {

        float tilingX = (width / 2.0f) / textureScale.x;
        float tilingY = (height / 2.0f) / textureScale.y;
        glUniform2f(tilingLoc, tilingX, tilingY);
    }

    GLuint hasTextureLoc = glGetUniformLocation(shader.ID, "hasTexture");
    if (hasTextureLoc != -1) {
        glUniform1i(hasTextureLoc, hasTexture);
    }

    if (hasTexture) {
        wallTexture.Bind();
        wallTexture.texUnit(shader, "wallTexture", 0);
    }

    wallVAO.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    wallVAO.Unbind();

    if (hasTexture) {
        wallTexture.Unbind();
    }
}

void buildingWall::setTextureScale(float scaleX, float scaleY) {
    textureScale = vec2(scaleX, scaleY);
}

void buildingWall::setPosition(vec3 newPos) {
    position = newPos;
}

void buildingWall::setRotation(vec3 newRot) {
    rotation = newRot;
}

void buildingWall::setScale(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
    Delete();
    setupGeometry();
}

void buildingWall::move(vec3 offset) {
    position += offset;
}

void buildingWall::rotate(vec3 angles) {
    rotation += angles;
}

void buildingWall::Delete() {
    wallVAO.Delete();
    wallVBO.Delete();
    wallEBO.Delete();
    if (hasTexture) {
        wallTexture.Delete();
    }
}