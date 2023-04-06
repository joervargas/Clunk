#pragma once

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>
#include <SDL2/SDL.h>
#include <Defines/Defines.h>

// #include <ClVkLoader/ClVkLoader.h>

#include "ClVkInstance.h"
#include "ClVkValidationLayers.h"
#include "ClVkSurface.h"
#include "ClVkPhysicalDevice.h"
#include "ClVkQueueFamilies.h"
#include "ClVkDevice.h"
#include "ClVkMemAllocator.h"

namespace Clunk
{

    /**
     * @brief Vulkan Context. Initializes and contains handles to ClVKDevice, ClVkMemAllocator, ClVkPhysicalDevice, ClVKQueueFamilies, ClVkSurface, ClVkInstance and ClVKValidationLayers
     * 
     */
    class ClVkContext
    {
    public:

        /**
         * @brief Create Vulkan Context. Initializes handles to ClVKDevice, ClVkMemAllocator, ClVkPhysicalDevice, ClVKQueueFamilies, ClVkSurface, ClVkInstance and ClVKValidationLayers
         * 
         * @return true 
         * @return false 
         */
        bool Create();

        /**
         * @brief Destroys the Vulkan context with all of its handles.
         * 
         */
        void Destroy();

        /**
         * @brief Get the Singleton Instance of ClVkContext.
         * 
         * @return ClVkContext* 
         */
        static ClVkContext* Get() { return &ClVkContext::m_ClVkContext; }

        ClVkInstance* Instance;
        ClVkValidationLayers* ValidationLayers;
        ClVkSurface* Surface;
        ClVkPhysicalDevice* PhsyicalDevice;
        ClVkQueueFamilies* QueueFamilies;
        ClVkDevice* Device;
        ClVkMemAllocator* MemAllocator;

    protected:


    private:

        ClVkContext() {};

        static ClVkContext m_ClVkContext;
        // LIBRARY_TYPE m_vk_lib;

    };

}