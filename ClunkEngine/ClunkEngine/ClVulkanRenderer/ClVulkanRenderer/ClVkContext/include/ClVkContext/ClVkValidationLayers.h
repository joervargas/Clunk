#pragma once

// #include <ClVkLoader/ClVkLoader.h>
#include <vulkan/vulkan.h>
#include <vector>

#include <PCH/pch.h>
#include <Core/Exception.h>
#include <Core/Logger.h>

#ifdef NDEBUG
    const b8 ENABLE_VALIDATION_LAYERS = false;
#else
    const b8 ENABLE_VALIDATION_LAYERS = true;
#endif

const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};


class ClVkValidationLayers
{
public:

    void SetupDebugMessenger();

    void Destroy();

    static ClVkValidationLayers* GetClVkValidationLayers() { return &m_ClVkValidationLayers; }

    static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;
    }

    static b8 CheckValidationLayerSupport()
    {
        // Get the number of available validation layers.
        u32 layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        // Populate a vector with a list of available layers.
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for(const char* layerName : VALIDATION_LAYERS)
        {
            b8 layerFound{false};
            for(const auto &layerProperties : availableLayers)
            {
                if(strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if(!layerFound)
            {
                return false;
            }
        }
        return true;
    }


    
protected:


private:

    ClVkValidationLayers() {};
    
    #ifndef NDEBUG
        VkDebugUtilsMessengerEXT m_debugMessenger;
    #endif

    static ClVkValidationLayers m_ClVkValidationLayers;


    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
        )
    {
        if(messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        {
            std::stringstream msg;
            switch (messageSeverity)
            {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                msg << "Validation Layer: " << pCallbackData->pMessage;
                CLOG_INFO(msg.str());
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                msg << "Validation Layer: " << pCallbackData->pMessage;
                CLOG_WARN(msg.str());
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                msg << "Validation Layer: " << pCallbackData->pMessage;
                // CLOG_ERROR(msg.str());
                THROW_EXCEPTION(String(msg.str()));
                break;
            default:
                break;
            }
        }
        return VK_FALSE;
    }

};