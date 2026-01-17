//
//  vertex.glsl
//  OpenGLProject
//
//  Created by Ameen on 1/5/26.
//

#version 330 core

// Vertex inputs
layout(location = 0) in vec3 aPos;    // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal
layout(location = 2) in vec2 aTex;    // Texture coordinate

// Outputs to fragment shader
out vec3 FragPos;      // Position in world space
out vec3 Normal;       // Normal in world space
out vec2 texCoord;     // Texture coordinates

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    // Transform normal correctly
    Normal = mat3(transpose(inverse(model))) * aNormal;

    texCoord = aTex;

    gl_Position = proj * view * worldPos;
}

