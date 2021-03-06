#version 450

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec3 color;
layout (location = 1) in vec3 posWorld;
layout (location = 2) in vec3 normalWorld;

layout (set = 0, binding = 0) uniform GlobalUbo{
    mat4 projectionViewMatrix;
    vec4 ambientLight;
    vec3 lightPosition;
    vec4 lightColor;
} ubo;

layout (push_constant) uniform Push{
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;


void main(){
    
    vec3 directionToLight = ubo.lightPosition - posWorld;
    float attenuation = 1.f / dot(directionToLight, directionToLight);

    vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
    vec3 ambientLight = ubo.ambientLight.xyz * ubo.ambientLight.w;
    vec3 diffuseLight = lightColor * max(dot(normalize(normalWorld), normalize(directionToLight)), 0);

    
    outColor = vec4((ambientLight + diffuseLight) * color, 1.);
}