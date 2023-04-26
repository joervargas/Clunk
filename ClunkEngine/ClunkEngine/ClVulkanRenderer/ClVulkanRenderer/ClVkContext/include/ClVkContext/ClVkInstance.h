#pragma once

#include <vulkan/vulkan.h>
#include <Defines/Defines.h>
#include <PCH/pch.h>

// #include <ClVkLoader/ClVkLoader.h>

#ifdef PLATFORM_SDL
    #include <SDL.h>
    #include <SDL_vulkan.h>
#endif


/**
 * @brief Handle for VkInstance with wrappers for VkInstance related functions.
 * 
 */
class ClVkInstance
{
public:

    #ifdef PLATFORM_SDL
        void Create(SDL_Window* Window);
        std::vector<const char*> GetRequiredExtensions(SDL_Window* Window);
    #else
        void Create();
    #endif

    void Destroy();

    static ClVkInstance* Get() { return &m_ClVkInstance; }
    
    /**
     * @brief Returns a pointer to a VkInstance.
     * @return VkInstance* 
     */
    VkInstance* Handle() { return &m_instance; }

    u32 GetVersion() const { return m_version; }


private:

    ClVkInstance()
    {
        m_EngineName = "Clunk VkEngine";
        m_AppName = "Clunk Engine";
    }
    ~ClVkInstance() {};
    
    static ClVkInstance m_ClVkInstance;
    VkInstance m_instance;

    String m_EngineName;
    String m_AppName;
    u32 m_version{VK_API_VERSION_1_3};

public:

    #ifdef PLATFORM_SDL
        SDL_bool CreateSurface(SDL_Window* Window, VkSurfaceKHR* Surface) { return SDL_Vulkan_CreateSurface(Window, m_instance, Surface); }
    #else
        void CreateSurface();
    #endif
    void DestroySurface(VkSurfaceKHR& Surface, VkAllocationCallbacks* pAllocator) { vkDestroySurfaceKHR(m_instance, Surface, pAllocator); }

    VkResult EnumeratePhysDevices(u32* Count, VkPhysicalDevice* pPhysicalDevices) { return vkEnumeratePhysicalDevices(m_instance, Count, pPhysicalDevices); };

};