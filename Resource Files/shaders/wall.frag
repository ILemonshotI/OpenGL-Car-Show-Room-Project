#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D wallTexture;
uniform bool hasTexture;
uniform vec3 color = vec3(0.8, 0.8, 0.8);

// Material properties
uniform vec3 ambientColor = vec3(0.1, 0.1, 0.1);
uniform vec3 diffuseColor = vec3(0.8, 0.8, 0.8);
uniform vec3 specularColor = vec3(0.5, 0.5, 0.5);
uniform float shininess = 32.0;

// Camera position
uniform vec3 viewPos;

// Light struct
struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
    bool enabled;
    int type;           // 0 = point, 1 = directional, 2 = spot
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

#define MAX_LIGHTS 20
uniform Light lights[MAX_LIGHTS];
uniform int numLights;

// Function to calculate lighting for each light type
vec3 calculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 materialColor) {
    if (!light.enabled) return vec3(0.0);
    
    vec3 lightDir;
    float attenuation = 1.0;
    
    // Calculate light direction and attenuation based on type
    if (light.type == 1) { // Directional light
        lightDir = normalize(-light.direction);
        attenuation = 1.0; // No attenuation for directional lights
    } 
    else { // Point or spot light
        lightDir = normalize(light.position - fragPos);
        float distance = length(light.position - fragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + 
                             light.quadratic * (distance * distance));
        
        // Spot light specific calculations
        if (light.type == 2) { // Spot light
            float theta = dot(lightDir, normalize(-light.direction));
            float epsilon = light.cutOff - light.outerCutOff;
            float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
            attenuation *= intensity;
        }
    }
    
    // Diffuse component
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * diff * diffuseColor * materialColor * light.intensity;
    
    // Blinn-Phong specular component
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = light.color * spec * specularColor * light.intensity;
    
    return (diffuse + specular) * attenuation;
}

void main()
{
    // Base color from texture or uniform
    vec3 baseColor;
    if (hasTexture) {
        baseColor = texture(wallTexture, TexCoord).rgb;
    } else {
        baseColor = color;
    }
    
    // Normalize vectors
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Ambient component (independent of lights)
    vec3 ambient = ambientColor * baseColor;
    
    // Calculate lighting from all active lights
    vec3 lighting = vec3(0.0);
    for (int i = 0; i < numLights; i++) {
        lighting += calculateLight(lights[i], norm, FragPos, viewDir, baseColor);
    }
    
    // Combine lighting components
    vec3 result = ambient + lighting;
    
    // Gamma correction (optional)
    result = pow(result, vec3(1.0/2.2));
    
    FragColor = vec4(result, 1); 

    return;
}