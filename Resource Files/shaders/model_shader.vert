#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

out vec3 crntPos;
out vec3 Normal;
out vec3 color;
out vec2 texCoord;

uniform mat4 camMatrix;
uniform mat4 model;
// Note: Usually you combine T, R, and S into the 'model' matrix on the CPU side.
// If you must pass them individually, ensure the order is correct.

void main()
{
    // 1. Calculate world position
    // standard order: Translation * Rotation * Scale
    crntPos = vec3(model * vec4(aPos, 1.0));

    // 2. Transform Normals to World Space
    // This ensures lighting stays correct when the object rotates.
    // mat3(transpose(inverse(model))) is the "Normal Matrix"
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);


    // 3. Pass through color and texCoords
    color = aColor;
    texCoord = aTex; 
    
    // 4. Final clip-space position
    gl_Position = camMatrix * vec4(crntPos, 1.0);
}