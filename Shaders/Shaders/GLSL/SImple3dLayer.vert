#version 460

// layout(binding = 0) uniform WorldTransformObject
// {
//     mat4 model;
//     mat4 view;
//     mat4 proj;
// } wto;

layout(binding = 0) uniform CameraUniform
{
    mat4 view;
    mat4 proj;
} camera_ubo;

layout(binding = 1) uniform ModelSpace
{
    mat4 data;
} model;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout( location = 0 ) out vec3 fragColor; 
layout( location = 1 ) out vec2 outTexCoord;

void main()
{
    // gl_Position = wto.proj * wto.view * wto.model * vec4(inPos, 1.0);
    gl_Position = camera_ubo.proj * camera_ubo.view * model.data * vec4(inPos, 1.0);

    fragColor = inColor;
    outTexCoord = inTexCoord;
}