#include "Ground.h"
#include <iostream>

buildingGround::buildingGround(float size, const char* texturePath)
    : size(size), hasTexture(texturePath != nullptr), groundVAO(), groundVBO(), groundEBO()
{
    setupGround(size);
    if (hasTexture) {
        groundTexture = buildingTexture(texturePath, GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
        groundTexture.Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        groundTexture.Unbind();
    }
}

void buildingGround::setupGround(float size) {
    float halfSize = size / 2.0f;
    float vertices[] = {
        -halfSize, 0.0f, -halfSize,  0.0f, 0.0f,
         halfSize, 0.0f, -halfSize,  1.0f, 0.0f,
         halfSize, 0.0f,  halfSize,  1.0f, 1.0f,
        -halfSize, 0.0f,  halfSize,  0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    groundVAO.Bind();

    groundVBO = buildingVBO(vertices, sizeof(vertices));
    groundEBO = buildingEBO(indices, sizeof(indices));

    groundVAO.LinkAttrib(groundVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    groundVAO.LinkAttrib(groundVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    groundVAO.Unbind();
    groundVBO.Unbind();
    groundEBO.Unbind();
}

void buildingGround::Draw(Shader& shader, mat4 view, mat4 projection) {
    shader.Activate();

    mat4 model = mat4(1.0f);
    model = translate(model, vec3(0.0f, 0.0f, 0.0f));

    GLuint modelLoc = glGetUniformLocation(shader.ID, "model");
    GLuint viewLoc = glGetUniformLocation(shader.ID, "view");
    GLuint projLoc = glGetUniformLocation(shader.ID, "projection");
    GLuint tilingLoc = glGetUniformLocation(shader.ID, "tiling");
    GLuint hasTextureLoc = glGetUniformLocation(shader.ID, "hasTexture");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

    if (tilingLoc != -1) {
        glUniform1f(tilingLoc, size / 2.0f);
    }

    if (hasTextureLoc != -1) {
        glUniform1i(hasTextureLoc, hasTexture);
    }

    if (hasTexture) {
        groundTexture.Bind();
        groundTexture.texUnit(shader, "groundTexture", 0);
    }

    groundVAO.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    groundVAO.Unbind();

    if (hasTexture) {
        groundTexture.Unbind();
    }
}

void buildingGround::Delete() {
    groundVAO.Delete();
    groundVBO.Delete();
    groundEBO.Delete();
    if (hasTexture) {
        groundTexture.Delete();
    }
}