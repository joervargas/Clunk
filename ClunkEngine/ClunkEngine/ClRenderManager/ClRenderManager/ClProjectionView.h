#pragma once

// #include <glm/glm.hpp>

#include <ClMath/Mat4.h>

<<<<<<< HEAD:ClunkEngine/ClunkEngine/ClVulkanRenderer/ClVulkanRenderer/ClVkTransforms.h
// struct ClVkTransforms
// {
//     // glm::mat4 model;
//     // glm::mat4 view;
//     // glm::mat4 proj;
    
//     Clunk::Mat4 model;
//     Clunk::Mat4 view;
//     Clunk::Mat4 proj;
// };

struct ClVkTransforms
=======
struct ClProjectionView
>>>>>>> c88e78f524e931970af550dd9b3e286bdbe2d650:ClunkEngine/ClunkEngine/ClRenderManager/ClRenderManager/ClProjectionView.h
{
    Clunk::Mat4 view;
    Clunk::Mat4 proj;
};