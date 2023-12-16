#ifndef VULKAN_INSTANCE_H
#define VULKAN_INSTANCE_H

#include <vulkan/vulkan.h>
#include <PCH/pch.h>
#include <SDL.h>

#ifndef NDEBUG
    const bool VALIDATION = true;
#else
    const bool VALIDATION = false;
#endif

namespace Clunk::Vk
{

    /**
     * @brief Create a VkInstance object
     * 
     * @param pInstance A pointer to VkInstance handle
     * @param pAppName Application name
     * @param AppVersion u32 recieved from VK_MAKE_API_VERSION()
     * @param pEngineName Engine name
     * @param EngineVersion u32 recieved from VK_MAKE_API_VERSION()
     */
    void create_vk_instance(VkInstance* pInstance, u32 Vk_API_Version, const char* pAppName, u32 AppVersion, const char* pEngineName, u32 EngineVersion);

    /**
     * @brief Sets the function pointers for VkValidationLayer Callbacks
     * 
     * @param Instance VkInstance Handle
     * @param pMessenger a pointer to VkDebugUtilsMessengerEXT struct
     * @param pReportCallback a pointer to a VkDebugReportCallbackEXT struct
     * @return true 
     * @return false 
     */
    bool create_debug_callbacks(const VkInstance Instance, VkDebugUtilsMessengerEXT* pMessenger, VkDebugReportCallbackEXT* pReportCallback);

    /**
     * @brief Sets function pointers to destroy VkValidationLayer callbacks
     * 
     * @param Instance VkInstance handle
     * @param pMessenger a pointer to a VkDebugUtilsMessengerEXT struct
     * @param pReportCallback a pointer to a VkDebugReportCallbackEXT struct
     */
    void destroy_debug_calbacks(const VkInstance Instance, VkDebugUtilsMessengerEXT* pMessenger, VkDebugReportCallbackEXT* pReportCallback);

    /**
     * @brief Creates a VkSurface from an SDL_Window
     * @param Window SDL_Window pointer
     * @param Instance a VkInstance handle
     * @param Surface a pointer to a VkSurfaceKHR handle to set
    */
    void create_vk_surface(SDL_Window* Window, const VkInstance Instance, VkSurfaceKHR* Surface);

    struct ClVkLoader
    {
        VkInstance Instance = nullptr;
        VkSurfaceKHR Surface = nullptr;
        VkDebugUtilsMessengerEXT Messenger;
        VkDebugReportCallbackEXT ReportCallback;

        u32 Vk_API_Version = VK_API_VERSION_1_3;
    };

    /**
     * @brief Create a VulkanLoader struct
     * 
     * @param Window SDL_Window pointer
     * @return ClVkLoader 
     */
    ClVkLoader cl_create_vk_loader(SDL_Window* Window, const char* AppName, u32 AppVersion);

    /**
     * @brief Destoy the contents of the ClVkLoader struct
     * 
     * @param pVkLoader a pointer to the ClVkLoader struct to be destroyed
     */
    void cl_destroy_vk_loader(ClVkLoader* pVkLoader);

}


#endif
