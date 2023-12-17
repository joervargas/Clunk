#pragma once

#include <Core/Logger.h>
#include <vulkan/vk_enum_string_helper.h>


namespace Clunk::Vk
{
    /**
     * Checks the VkResult of vulkan functions. Throws errors if found.
     */
    #define VK_CHECK(x)                                     \
        {                                                   \
            VkResult err = x;                               \
            if(err)                                         \
            {                                               \
                THROW_EXCEPTION("Vulkan Error: %s", string_VkResult(err));   \
            }                                               \
        }  
}