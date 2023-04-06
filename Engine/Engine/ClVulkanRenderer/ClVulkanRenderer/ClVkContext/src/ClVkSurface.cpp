#include "ClVkSurface.h"
#include "ClVkInstance.h"
#include "ClVkPhysicalDevice.h"

#include <Core/Logger.h>
#include <Core/Exception.h>

ClVkSurface ClVkSurface::m_ClVkSurface;

#ifdef PLATFORM_SDL
    void ClVkSurface::Create(SDL_Window* Window)
    {
        CLOG_INFO("Creating SDL Vulkan Surface...");

        ClVkInstance* Instance = ClVkInstance::Get();
        if(!Instance->Handle())
        {
            CLOG_WARN("Instance pointer is null!");
        }
        if(!Window)
        {
            CLOG_WARN("Instance pointer is null!");
        }

        SDL_bool bSuccess{SDL_FALSE};
        // bSuccess = SDL_Vulkan_CreateSurface(Window, *Instance->Get(), &m_surface);
        bSuccess = Instance->CreateSurface(Window, &m_surface);
        if(!bSuccess)
        {
            THROW_EXCEPTION("Failed to Create SDL Vulkan Surface");
        }

        CLOG_INFO("SDL Vulkan Surface created.");
    }

#else

    void ClVkSurface::Create()
    {}


#endif

void ClVkSurface::Destroy()
{
    CLOG_INFO("Destroying Surface...");
    ClVkInstance* Instance = ClVkInstance::Get();
    Instance->DestroySurface(m_surface, nullptr);
    // vkDestroySurfaceKHR(*Instance->Get(), m_surface, nullptr);
    CLOG_INFO("Surface Destroyed.");
}

void ClVkSurface::QueryDetails(const VkPhysicalDevice& PhysDevice)
{

    // ClVkPhysicalDevice* PhysDevice = ClVkPhysicalDevice::GetClPhysDevice();
    // if(!PhysDevice) { THROW_EXCEPTION("Physical Device not set!"); }

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysDevice, m_surface, &details.capabilities);

    u32 surfaceFormatCount{};
    vkGetPhysicalDeviceSurfaceFormatsKHR(PhysDevice, m_surface, &surfaceFormatCount, nullptr);
    if(surfaceFormatCount != 0)
    {
        details.formats.resize(surfaceFormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(PhysDevice, m_surface, &surfaceFormatCount, details.formats.data());
    }

    u32 presentModeCount{};
    vkGetPhysicalDeviceSurfacePresentModesKHR(PhysDevice, m_surface, &presentModeCount, nullptr);
    if(presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(PhysDevice, m_surface, &presentModeCount, details.presentModes.data());
    }
}
