#pragma once

#include <Defines/Defines.h>
#include <Core/Exception.h>
#include <Core/Logger.h>


namespace vk_ext
{

    // #if defined PLATFORM_WINDOWS
    //     #include <Windows.h>
    //     #define LIBRARY_TYPE HMODULE
    // #else
    //     #include <dlfcn.h>
    //     #define LIBRARY_TYPE void*
    // #endif
    
    // static b8 ConnectWithVulkanLibrary(LIBRARY_TYPE& Vk_Lib)
    // {
    //     #if defined PLATFORM_WINDOWS
    //         Vk_Lib = LoadLibraryA("vulkan-1.dll");
    //     #else
    //         Vk_Lib = dlopen("libvulkan.so.1", RTLD_NOW);
    //     #endif

    //     if(Vk_Lib == nullptr)
    //     {
    //         THROW_EXCEPTION("Could not connect with a Vulkan Runtime Library!");
    //         return false;
    //     }
    //     return true;
    // }

    // static b8 LoadExportFunctions(LIBRARY_TYPE& Vk_Lib)
    // {
    //     #if defined PLATFORM_WINDOWS
    //         #define LoadFunction GetProcAddress
    //     #else
    //         #define LoadFunction dlsym
    //     #endif

    //     #define EXPORTED_VK_FUNC(name) \
    //         name = (PFN_##name)LoadFunction(Vk_Lib, #name); \
    //         if(name == nullptr) \
    //         { \
    //             THROW_EXCEPTION("Could not load exported Vulkan function named: %s", #name); \
    //             return false; \
    //         } 

    //     #include "ClVkFunctions.inl"
    //     return true;
    // }

    // static b8 LoadGlobalFunctions()
    // {
    //     #define GLOBAL_VK_FUNC(name) \
    //         name = (PFN_##name)vkGetInstanceProcAddr(nullptr, #name); \
    //         if(name == nullptr) \
    //         { \
    //             THROW_EXCEPTION("Could not load global Vulkan function named: %s", #name); \
    //             return false; \
    //         }
        
    //     #include "ClVkFunctions.inl"

    //     return true;
    // }

    static b8 LoadInstanceFunctions(VkInstance& Instance, std::vector<const char *> enabled_extensions)
    {
        #define INSTANCE_VK_FUNC(name) \
            name = (PFN_##name)vkGetInstanceProcAddr(Instance, #name); \
            if(name == nullptr) \
            { \
                THROW_EXCEPTION("Could not load instance Vulkan function named: %s", #name); \
                return false; \
            } \
        
        #define INSTANCE_VK_EXT_FUNC(name, extension) \
            for(auto& enabled_extension : enabled_extensions) \
            { \
                if(String(enabled_extension) == String(extension)) \
                { \
                    name = (PFN_##name)vkGetInstanceProcAddr(Instance, #name); \
                } \
                if(&name == nullptr) \
                { \
                    THROW_EXCEPTION("Could not load instance Vulkan extension function named: %s", #name); \
                    return false; \
                } \
            } \
        
        #include "ClVkFunctions.inl"
        return true;
    }

    static b8 LoadDeviceFunctions(VkDevice& Device, std::vector<const char*> enabled_extensions)
    {
        #define DEVICE_VK_FUNC(name) \
            name = (PFN_##name)vkGetDeviceProcAddr(Device, #name); \
            if(name == nullptr) \
            { \
                THROW_EXCEPTION("Could not load device Vulkan function named: %s", #name); \
                return false; \
            } \

        #define DEVICE_VK_EXT_FUNC(name, extension) \
            for(auto& enabled_extension : enabled_extensions) \
            { \
                if(String(enabled_extension) == String(extension)) \
                { \
                    name = (PFN_##name)vkGetDeviceProcAddr(Device, #name); \
                } \
                if(&name == nullptr) \
                { \
                    THROW_EXCEPTION("Could not load device Vulkan extension function named: %s", #name); \
                    return false; \
                } \
            } \

        #include "ClVkFunctions.inl"
        return true;
    }

    // static void CloseLibrary(LIBRARY_TYPE& Vk_Lib)
    // {
    //     #if defined PLATFORM_WINDOWS
    //         FreeLibrary(Vk_Lib);
    //     #else
    //         dlclose(Vk_Lib);
    //     #endif
    // }
}
