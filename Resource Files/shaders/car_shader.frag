//
//  fragment.glsl
//  OpenGLProject
//
//  Created by Ameen on 1/5/26.
//

#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;

uniform sampler2D tex0;
uniform vec4 objectColor;  // RGBA
uniform bool useTexture;   // true -> texture, false -> flat color

// Simple directional light
uniform vec3 lightDir = normalize(vec3(-0.5, -1.0, -0.3));
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

// ---- Glass / Fresnel uniforms ----
uniform bool isGlass = false;
uniform float glassStrength = 1.0;
uniform float glassOpacity = 0.5;

// ---- PBR uniforms ----
uniform bool isPBR = false;
uniform float metalness = 0.0;
uniform float roughness = 0.5;
uniform vec3 albedo = vec3(1.0);

// ---------- PBR Helper Functions ----------
const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float rough)
{
    float a = rough * rough;
    float a2 = a * a;
    float NdotH = max(dot(N,H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float rough)
{
    float r = rough + 1.0;
    float k = (r*r)/8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float rough)
{
    return GeometrySchlickGGX(max(dot(N,V),0.0), rough) *
           GeometrySchlickGGX(max(dot(N,L),0.0), rough);
}

// ---------- Main ----------
void main()
{
    vec3 norm = normalize(Normal);

    // ---------- Base color ----------
    vec3 baseColor = objectColor.rgb;

    if(useTexture)
    {
        // Always sample the texture safely
        baseColor = texture(tex0, texCoord).rgb;
    }

    // ---------- Classic lighting ----------
    vec3 finalColor = baseColor; // start with baseColor directly

    if(!isPBR) // only apply classic lighting if not PBR
    {
        float diff = dot(norm, -lightDir);
        diff = diff * 0.5 + 0.5;         // remap [-1,1] -> [0,1]
        diff = clamp(diff, 0.2, 1.0);    // avoid full dark
        vec3 diffuse = diff * lightColor;
        vec3 ambient = 0.25 * lightColor;

        finalColor *= (ambient + diffuse);
    }

    // ---------- Glass ----------
    if(isGlass)
    {
        vec3 viewDir = normalize(-FragPos);
        float fresnel = pow(1.0 - max(dot(norm, viewDir), 0.0), glassStrength);
        vec3 envReflection = mix(finalColor, vec3(1.0), 0.3);
        finalColor = mix(finalColor, envReflection, fresnel);
    }

    // ---------- PBR ----------
    if(isPBR)
    {
        vec3 V = normalize(-FragPos);
        vec3 L = normalize(-lightDir);
        vec3 H = normalize(V + L);

        vec3 albedoColor = baseColor * albedo;

        vec3 F0 = vec3(0.04);
        F0 = mix(F0, albedoColor, metalness);

        float NDF = DistributionGGX(norm, H, roughness);
        float G   = GeometrySmith(norm, V, L, roughness);
        vec3  F   = fresnelSchlick(max(dot(H,V),0.0), F0);

        vec3 numerator = NDF * G * F;
        float denom = 4.0 * max(dot(norm,V),0.0) * max(dot(norm,L),0.0) + 0.001;
        vec3 specular = numerator / denom;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metalness;

        float NdotL = max(dot(norm,L),0.0);
        vec3 diffusePBR = kD * albedoColor / PI;

        vec3 radiance = lightColor;
        vec3 pbrColor = (diffusePBR + specular) * radiance * NdotL;
        vec3 ambientPBR = vec3(0.03) * albedoColor;

        finalColor = ambientPBR + pbrColor;
    }

    // ---------- Output ----------
    float alpha = isGlass ? glassOpacity : objectColor.a;
    FragColor = vec4(finalColor, alpha);
}
