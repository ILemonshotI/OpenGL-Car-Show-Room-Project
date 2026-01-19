#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D groundTexture;
uniform bool hasTexture;

void main()
{
    if (hasTexture) {
        FragColor = texture(groundTexture, TexCoord);
    } else {
        FragColor = vec4(0.5, 0.5, 0.5, 1.0);
    }
}