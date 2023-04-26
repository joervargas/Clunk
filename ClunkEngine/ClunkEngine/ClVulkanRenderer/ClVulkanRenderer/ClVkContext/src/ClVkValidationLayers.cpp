#include "ClVkValidationLayers.h"
#include "ClVkInstance.h"

VkResult CreateDebugUtilsMessengerEXT(
        VkInstance& Instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger
    ) 
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT");
    if(func != nullptr)
    {
        return func(Instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(
        VkInstance& Instance,
        VkDebugUtilsMessengerEXT& DebugMessenger,
        const VkAllocationCallbacks* pAllocator
    )
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");
    if(func != nullptr)
    {
        func(Instance, DebugMessenger, pAllocator);
    }
}


ClVkValidationLayers ClVkValidationLayers::m_ClVkValidationLayers;

void ClVkValidationLayers::SetupDebugMessenger()
{
    CLOG_INFO("Setting up Debug Messenger for Validation Layers...");
    if(!ENABLE_VALIDATION_LAYERS) { return; }
    ClVkInstance* Instance = ClVkInstance::Get();

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);

    VK_CHECK(CreateDebugUtilsMessengerEXT(*Instance->Handle(), &createInfo, nullptr, &m_debugMessenger));
    CLOG_INFO("Debug Messenger for Validation Layers set.");
}

void ClVkValidationLayers::Destroy()
{
    if(!ENABLE_VALIDATION_LAYERS) { return; }

    ClVkInstance* Instance = ClVkInstance::Get();
    if(!Instance)
    {
        CLOG_ERROR("Pointer to clVInstance null!");
    }
    DestroyDebugUtilsMessengerEXT(*Instance->Handle(), m_debugMessenger, nullptr);
}
