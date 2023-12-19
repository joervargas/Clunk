#version 460

layout(binding = 0) uniform WorldTransformObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
} wto;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoords;

layout( location = 0 ) out vec3 fragColor; 
layout( location = 1 ) out vec2 outTexCoords;

void main()
{
    gl_Position = wto.proj * wto.view * wto.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    outTexCoord = inTexCoord;
}