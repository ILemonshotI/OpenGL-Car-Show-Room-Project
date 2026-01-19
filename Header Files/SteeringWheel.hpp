//
//  SteeringWheel.hpp
//  OpenGLProject
//
//  Created by Ameen on 1/16/26.
//

#ifndef STEERINGWHEEL_HPP
#define STEERINGWHEEL_HPP

#include "Object.hpp"
#include "Cylinder.hpp"
#include "Tcentre.hpp"
#include <vector>

class SteeringWheel : public Object
{
public:
    SteeringWheel(
        Shader* shader,
        Texture* texture,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotationAxis = glm::vec3(1, 0, 0),
        float rotationSpeed = 0.0f,
        float alpha = 1.0f
    );

    ~SteeringWheel();


    void drawWithParent(const glm::mat4& parent,
        const glm::mat4& view,
        const glm::mat4& proj) override;


private:
    std::vector<Cylinder*> rim;
    Tcentre* centre;
    glm::vec3 TOffsets[1];


};

#endif
