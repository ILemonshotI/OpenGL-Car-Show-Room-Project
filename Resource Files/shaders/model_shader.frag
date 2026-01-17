#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs from the vertex shader
in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

// Texture samplers
uniform sampler2D diffuse0;
uniform sampler2D specular0;

// Light info
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

// -------------------- LIGHT FUNCTIONS -------------------- //

vec4 pointLight()
{
    vec4 baseColor = texture(diffuse0, texCoord) * vec4(color, 1.0);

    vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);
    float a = 3.0;
    float b = 0.7;
    float inten = 1.0 / (a * dist * dist + b * dist + 1.0);

    float ambient = 0.20;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightVec);
    float diffuse = max(dot(normal, lightDir), 0.0);

    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    float specular = spec * specularStrength;

    return (baseColor * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 direcLight()
{
    vec4 baseColor = texture(diffuse0, texCoord) * vec4(color, 1.0);

    float ambient = 0.20;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 0.0)); // directional light
    float diffuse = max(dot(normal, lightDir), 0.0);

    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    float specular = spec * specularStrength;

    return (baseColor * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
    vec4 baseColor = texture(diffuse0, texCoord) * vec4(color, 1.0);

    float outerCone = 0.90;
    float innerCone = 0.95;
    float ambient = 0.20;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - crntPos);
    float diffuse = max(dot(normal, lightDir), 0.0);

    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    float specular = spec * specularStrength;

    float angle = dot(vec3(0.0, -1.0, 0.0), -lightDir);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0, 1.0);

    return (baseColor * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

// -------------------- MAIN -------------------- //

void main()
{
 
     vec4 texColor = texture(diffuse0, texCoord);
    vec4 baseColor = vec4(texColor.rgb * color, texColor.a);

    // Discard fully transparent pixels
    if(baseColor.a < 0.1)
        discard;

    // Apply directional light
    vec4 lighting = direcLight() * vec4(1.0, 1.0, 1.0, baseColor.a);

    FragColor = lighting;
}
