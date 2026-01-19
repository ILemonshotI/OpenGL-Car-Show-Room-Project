#include "Skybox.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

GLuint loadCubemapTexture(std::vector<std::string> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = GL_RGB;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

buildingSkybox::buildingSkybox(std::vector<std::string> faceTextures) {
    GLuint cubemapID = loadCubemapTexture(faceTextures);

    skyboxVAO = buildingVAO();
    skyboxVBO = buildingVBO(skyboxVertices, sizeof(skyboxVertices));

    skyboxVAO.Bind();
    skyboxVAO.LinkAttrib(skyboxVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    skyboxVAO.Unbind();
    skyboxVBO.Unbind();
}

void buildingSkybox::Draw(Shader& shader, mat4 view, mat4 projection) {
    glDepthFunc(GL_LEQUAL);

    shader.Activate();

    mat4 viewNoTranslation = mat4(mat3(view));

    GLuint viewLoc = glGetUniformLocation(shader.ID, "view");
    GLuint projLoc = glGetUniformLocation(shader.ID, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(viewNoTranslation));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

    skyboxVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    skyboxVAO.Unbind();

    glDepthFunc(GL_LESS);
}

void buildingSkybox::Delete() {
    skyboxVAO.Delete();
    skyboxVBO.Delete();
    cubemapTexture.Delete();
}