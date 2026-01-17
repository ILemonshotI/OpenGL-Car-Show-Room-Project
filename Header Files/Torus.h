#ifndef TORUS_HPP
#define TORUS_HPP

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Torus {
public:
    Torus(float majorRadius, float minorRadius, int sectors, int sides);
    void Draw();

private:
    void generate();

    float R, r;
    int sectorCount, sideCount;

    GLuint VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif
