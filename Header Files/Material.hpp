//
//  Material.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/13/26.
//

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Shader.hpp"
#include <glm/gtc/matrix_transform.hpp>


namespace Material
{
    // ---- Glass ----
    void setGlass(Shader& shader, float strength, float opacity);
    void unSetGlass(Shader& shader);

    // ---- PBR ----
    void setPBR(Shader& shader, float metalness, float roughness,glm::vec3 albedo);
    void unSetPBR(Shader& shader);
}

#endif
