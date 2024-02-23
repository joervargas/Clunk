#pragma once

// #include <glm/glm.hpp>

#include <ClMath/Mat4.h>

// struct ClVkTransforms
// {
//     // glm::mat4 model;
//     // glm::mat4 view;
//     // glm::mat4 proj;
    
//     Clunk::Mat4 model;
//     Clunk::Mat4 view;
//     Clunk::Mat4 proj;
// };

// struct ClVkTransforms
struct ClProjectionView
{
    Clunk::Mat4 View;
    Clunk::Mat4 Proj;
};