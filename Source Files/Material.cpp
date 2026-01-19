//
//  Material.cpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#include "Material.hpp"
#include <glad/glad.h>


namespace Material
{
    // ---------------- GLASS ----------------
    void setGlass(Shader& shader, float strength, float opacity)
    {
        shader.Activate();
        glUniform1i(glGetUniformLocation(shader.ID, "isGlass"), GL_TRUE);
        glUniform1f(glGetUniformLocation(shader.ID, "glassStrength"), strength);
        glUniform1f(glGetUniformLocation(shader.ID, "glassOpacity"), opacity);
    }

    void unSetGlass(Shader& shader)
    {
        shader.Activate();
        glUniform1i(glGetUniformLocation(shader.ID, "isGlass"), GL_FALSE);
    }

    // ---------------- PBR ----------------
    void setPBR(Shader& shader, float metalness, float roughness, glm::vec3 albedo = glm::vec3(1.0f))
    {
        shader.Activate();
        glUniform1f(glGetUniformLocation(shader.ID, "metalness"), metalness);
        glUniform1f(glGetUniformLocation(shader.ID, "roughness"), roughness);
        glUniform3f(glGetUniformLocation(shader.ID, "albedo"), albedo.r, albedo.g, albedo.b);
    }


    void unSetPBR(Shader& shader)
    {
        shader.Activate();
        glUniform1i(glGetUniformLocation(shader.ID, "isPBR"), GL_FALSE);
    }
}
