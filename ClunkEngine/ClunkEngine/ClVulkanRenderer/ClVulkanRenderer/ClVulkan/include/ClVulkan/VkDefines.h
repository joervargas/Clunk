#pragma once

#include <Core/Logger.h>


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
                THROW_EXCEPTION("Vulkan Error: %s", err);   \
            }                                               \
        }  
}