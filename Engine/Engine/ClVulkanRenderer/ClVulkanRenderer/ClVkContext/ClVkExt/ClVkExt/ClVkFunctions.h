#pragma once

#include <vulkan/vulkan.h>

namespace vk_ext
{
    // #define EXPORTED_VK_FUNC(name) extern PFN_##name name;

    // #define GLOBAL_VK_FUNC(name) extern PFN_##name name;

    #define INSTANCE_VK_FUNC(name) extern PFN_##name name;

    #define INSTANCE_VK_EXT_FUNC(name, extension) extern PFN_##name name;

    #define DEVICE_VK_FUNC(name) extern PFN_##name name;

    #define DEVICE_VK_EXT_FUNC(name, extension) extern PFN_##name name;

    #include "ClVkFunctions.inl"
}

