#include "ClVkLoader.h"
#include "VkUtils.h"
#include "ClVkExt/ClVkExt.h"

#include <Core/Logger.h>
#include <vector>

#include <SDL_vulkan.h>


namespace Clunk::Vk
{

    void create_vk_instance(VkInstance *pInstance, u32 Vk_API_Version, const char* pAppName, u32 AppVersion, const char* pEngineName, u32 EngineVerson)
    {
        CLOG_INFO("Creating VkInstance handle...")

        const std::vector<const char*> validation_layers =
        {
            "VK_LAYER_KHRONOS_validation"
        };

        const std::vector<const char *> instance_extenstions =
        {
            VK_KHR_SURFACE_EXTENSION_NAME,
            #if defined(WIN32)
                "VK_KHR_win32_surface",
            #endif
            #if defined(__APPLE__)
                "VK_MVK_macos_surface",
            #endif
            #if defined(__linux__)
                "VK_KHR_xcb_surface",
                "VK_KHR_xlib_surface",
            #endif
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
        };

        const VkApplicationInfo appInfo =
        {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = pAppName,
            .applicationVersion = AppVersion,
            .pEngineName = pEngineName,
            .engineVersion = EngineVerson,
            .apiVersion = Vk_API_Version
        };

        const VkInstanceCreateInfo createInfo =
        {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = static_cast<u32>(validation_layers.size()),
            .ppEnabledLayerNames = validation_layers.data(),
            .enabledExtensionCount = static_cast<u32>(instance_extenstions.size()),
            .ppEnabledExtensionNames = instance_extenstions.data()
        };

        VK_CHECK(vkCreateInstance(&createInfo, nullptr, pInstance));
        vk_ext::LoadInstanceFunctions(*pInstance, instance_extenstions);
        
        CLOG_INFO("VkInstance handle created.");
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugMessengerCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT Severity,
            VkDebugUtilsMessageTypeFlagsEXT Type,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
        )
    {
        if(Severity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        {
            std::stringstream msg;
            switch (Severity)
            {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                msg << "Validation Layer: " << pCallbackData->pMessage;
                log_validation_layer(ELog_Level::INFO_LEVEL, msg.str());
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                msg << "Validation Layer: " << pCallbackData->pMessage;
                log_validation_layer(ELog_Level::WARN_LEVEL, msg.str());
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                msg << "Validation Layer: " << pCallbackData->pMessage;
                log_validation_layer(ELog_Level::ERROR_LEVEL, msg.str());
                break;
            default:
                break;
            }
        }
        return VK_FALSE;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(
            VkDebugReportFlagsEXT flags,
            VkDebugReportObjectTypeEXT objectType,
            uint64_t object,
            size_t location,
            int32_t messageCode,
            const char* pLayerPrefix,
            const char* pMessage,
            void* pUserData
        ) 
    {
        if(
            flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT && 
            flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT
        ) { return VK_FALSE; }

        if(flags & VK_DEBUG_REPORT_WARNING_BIT_EXT || flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
        {
            log_validation_layer(ELog_Level::WARN_LEVEL, "Validatin Report Callback (%s): %s", pLayerPrefix, pMessage);
        }
        if(flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
        {
            log_validation_layer(ELog_Level::ERROR_LEVEL, "Validation Report Callback (%s): %s", pLayerPrefix, pMessage);
        }

        return VK_FALSE;
    }

    bool create_debug_callbacks(const VkInstance Instance, VkDebugUtilsMessengerEXT *pMessenger, VkDebugReportCallbackEXT *pReportCallback)
    {
        CLOG_INFO("Creating Validation Layer Callbacks...");

        // Create messenger block
        {
            const VkDebugUtilsMessengerCreateInfoEXT messengerInfo =
            {
                .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                .pfnUserCallback = &VulkanDebugMessengerCallback,
                .pUserData = nullptr
            };
            auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT");
            VK_CHECK(func(Instance, &messengerInfo, nullptr, pMessenger));
        }
        // create report callback block
        {
            const VkDebugReportCallbackCreateInfoEXT reportCallbackInfo =
            {
                .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
                .pNext = nullptr,
                .flags = VK_DEBUG_REPORT_WARNING_BIT_EXT |
                    VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
                    VK_DEBUG_REPORT_ERROR_BIT_EXT |
                    VK_DEBUG_REPORT_DEBUG_BIT_EXT,
                .pfnCallback = &VulkanDebugReportCallback,
                .pUserData = nullptr
            };
            auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(Instance, "vkCreateDebugReportCallbackEXT");
            VK_CHECK(func(Instance, &reportCallbackInfo, nullptr, pReportCallback));
        }

        CLOG_INFO("Validation Callbacks created.");
        return false;
    }

    void destroy_debug_calbacks(const VkInstance& Instance, VkDebugUtilsMessengerEXT& Messenger, VkDebugReportCallbackEXT& ReportCallback)
    {
        // Destroy Messenger block
        {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");
            if(func != nullptr)
            {
                func(Instance, Messenger, nullptr);
            } else {
                CLOG_ERROR("vkDestroyDebugUtilsMessengerEXT funtion unable to load");
                exit(EXIT_FAILURE);
            }
        }
        // Destroy Report Callback
        {
            auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(Instance, "vkDestroyDebugReportCallbackEXT");
            if(func != nullptr)
            {
                func(Instance, ReportCallback, nullptr);
            } else {
                CLOG_ERROR("vkDestroyDebugReportCallbackEXT funtion unable to load");
                exit(EXIT_FAILURE);
            }
        }
    }

    void create_vk_surface(SDL_Window *Window, const VkInstance Instance, VkSurfaceKHR *Surface)
    {
        CLOG_INFO("Creating SDL Vulkan Surface...");

        SDL_bool bSuccess = SDL_FALSE;
        bSuccess = SDL_Vulkan_CreateSurface(Window, Instance, Surface);
        if(!bSuccess)
        {
            CLOG_ERROR("Failed to create SDL Vulkan Surface: %s", SDL_GetError());
        }

        CLOG_INFO("SDL Vulkan Surface created.");
    }

    ClVkLoader cl_create_vk_loader(SDL_Window *Window, const char* AppName, u32 AppVersion)
    {   
        CLOG_INFO("Creating VulkanLoader...");

        ClVkLoader vkLoader;
        create_vk_instance(&vkLoader.Instance, vkLoader.Vk_API_Version ,  AppName, AppVersion, "Clunk Engine", VK_MAKE_API_VERSION(0, 0, 1, 0));
        if( VALIDATION )
        {
            create_debug_callbacks(vkLoader.Instance, &vkLoader.Messenger, &vkLoader.ReportCallback);
        }
        create_vk_surface(Window, vkLoader.Instance, &vkLoader.Surface);

        CLOG_INFO("VulkanLoader created.");
        return vkLoader;
    }

    void cl_destroy_vk_loader(ClVkLoader* VkLoader)
    {
        CLOG_INFO("Destroying VulkanLoader struct...");

        vkDestroySurfaceKHR(VkLoader->Instance, VkLoader->Surface, nullptr);
        if( VALIDATION )
        {
            destroy_debug_calbacks(VkLoader->Instance, VkLoader->Messenger, VkLoader->ReportCallback);
        }
        vkDestroyInstance(VkLoader->Instance, nullptr);
        
        VkLoader->Surface = nullptr;
        VkLoader->Messenger = nullptr;
        VkLoader->ReportCallback = nullptr;
        VkLoader->Instance = nullptr;

        CLOG_INFO("VulkanLoader destroyed.");
    }

}
