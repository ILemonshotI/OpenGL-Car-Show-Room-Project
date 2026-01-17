#include "Torus.h"
#include <cmath>

Torus::Torus(float majorRadius, float minorRadius, int sectors, int sides)
    : R(majorRadius), r(minorRadius),
    sectorCount(sectors), sideCount(sides)
{
    generate();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Torus::generate()
{
    const float PI = acos(-1.0f);

    for (int i = 0; i <= sideCount; i++) {
        float sideAngle = i * 2 * PI / sideCount;
        float cosSide = cos(sideAngle);
        float sinSide = sin(sideAngle);

        for (int j = 0; j <= sectorCount; j++) {
            float sectorAngle = j * 2 * PI / sectorCount;
            float cosSector = cos(sectorAngle);
            float sinSector = sin(sectorAngle);

            float x = (R + r * cosSide) * cosSector;
            float y = (R + r * cosSide) * sinSector;
            float z = r * sinSide;

            float nx = cosSide * cosSector;
            float ny = cosSide * sinSector;
            float nz = sinSide;

            vertices.insert(vertices.end(),
                { x, y, z, nx, ny, nz });
        }
    }

    for (unsigned int i = 0; i < sideCount; i++) {
        for (unsigned int j = 0; j < sectorCount; j++) {
            unsigned int first = i * (sectorCount + 1) + j;
            unsigned int second = first + sectorCount + 1;

            indices.insert(indices.end(),
                { first, second, first + 1,
                  first + 1, second, second + 1 });
        }
    }
}

void Torus::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
