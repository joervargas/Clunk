#pragma once

#include <vulkan/vulkan.h>
#include <PCH/pch.h>

#ifdef PLATFORM_SDL
    #include <SDL.h>
    // #include <SDL2/SDL_vulkan.h>
#endif


class ClVkSurface
{
public:


    #ifdef PLATFORM_SDL
        void Create(SDL_Window* Window);
    #else
        void Create();
    #endif

    void Destroy();

    static ClVkSurface* Get() { return &m_ClVkSurface; }

    const VkSurfaceKHR* Handle() { return &m_surface; }

    struct SurfaceDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    } details;

    void QueryDetails(const VkPhysicalDevice& PhysDevice);


private:

    ClVkSurface() {};

    static ClVkSurface m_ClVkSurface;
    VkSurfaceKHR m_surface;
};