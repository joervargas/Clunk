#include "ClVkInstance.h"
#include "ClVkValidationLayers.h"

#include <Core/Logger.h>
#include <Core/Exception.h>

#include <ClVkExt/ClVkExt.h>

ClVkInstance ClVkInstance::m_ClVkInstance;

#ifdef PLATFORM_SDL

    void ClVkInstance::Create(SDL_Window* Window)
    {
        CLOG_INFO("Loading Vulkan Instance...");

        if(!Window) { THROW_EXCEPTION("Can't initialize Vulkan; Window Pointer null!"); }
        

        // if(!ClVkLoader::LoadExportFunctions(Vk_Lib))
        // {
        //     THROW_EXCEPTION("Failed to load export functions!");
        // }

        if(ENABLE_VALIDATION_LAYERS && !ClVkValidationLayers::CheckValidationLayerSupport())
        {
            THROW_EXCEPTION("CheckValidationLayerSupport() Validation Layers requested, but not available!");
        }

        // #if defined PLATFORM_WINDOWS

        // #elif defined PLATFORM_LINUX
        //     void* vulkan_library
        // #endif

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = m_AppName.c_str();
        appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        appInfo.pEngineName = m_EngineName.c_str();
        appInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        appInfo.apiVersion = m_version;

        auto extensions = GetRequiredExtensions(Window);
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        // extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if(ENABLE_VALIDATION_LAYERS)
        {
            createInfo.enabledLayerCount = static_cast<u32>(VALIDATION_LAYERS.size());
            createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();

            ClVkValidationLayers::PopulateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        // m_instance = vk::createInstance(createInfo);
        VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_instance));

        vk_ext::LoadInstanceFunctions(m_instance, extensions);
        // if(!ClVkLoader::LoadInstanceFunctions(m_instance, extensions))
        // {
        //     THROW_EXCEPTION("Failed to load Instance functions.");
        // }
        
        CLOG_INFO("Vulkan Instance Created.");
    }

    std::vector<const char*> ClVkInstance::GetRequiredExtensions(SDL_Window* Window)
    {
        u32 extensionCount{0};
        SDL_Vulkan_GetInstanceExtensions(Window, &extensionCount, nullptr);

        std::vector<const char*> extensions = {};
        if(ENABLE_VALIDATION_LAYERS)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            // extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }
        
        size_t additional_extension_count = extensions.size();
        extensions.resize(additional_extension_count + extensionCount);

        SDL_Vulkan_GetInstanceExtensions(Window, &extensionCount, extensions.data() + additional_extension_count);

        return extensions;
    }

#else

    void ClVkInstance::Create() {}

#endif

void ClVkInstance::Destroy()
{
    CLOG_INFO("Destroying Vulkan Instance...");

    vkDestroyInstance(m_instance, nullptr);
    
    CLOG_INFO("Vulkan Instance Destroyed.");
}