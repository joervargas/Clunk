#include "ClVkFunctions.h"

#include <vulkan/vulkan.h>

namespace vk_ext
{
    // #define EXPORTED_VK_FUNC(name) PFN_##name name;

    // #define GLOBAL_VK_FUNC(name) PFN_##name name;

    #define INSTANCE_VK_FUNC(name) PFN_##name name;

    #define INSTANCE_VK_EXT_FUNC(name, extension) PFN_##name name;

    #define DEVICE_VK_FUNC(name) PFN_##name name;

    #define DEVICE_VK_EXT_FUNC(name, extension) PFN_##name name;

    #include "ClVkFunctions.inl"
}

