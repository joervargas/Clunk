#include "ClVkContext.h"
#include <ClPlatforms/ClPlatform.h>


namespace Clunk
{
    ClVkContext ClVkContext::m_ClVkContext;

    bool ClVkContext::Create()
    {
        // if(!ClVkLoader::ConnectWithVulkanLibrary(m_vk_lib))
        // {
        //     THROW_EXCEPTION("Failed to connect to Vulkan library");
        // }
        
        ClPlatform* Platform = ClPlatform::Get();
        Instance = ClVkInstance::Get();
        Instance->Create(Platform->GetWindow());

        ValidationLayers = ClVkValidationLayers::GetClVkValidationLayers();
        ValidationLayers->SetupDebugMessenger();

        Surface = ClVkSurface::Get();
        Surface->Create(Platform->GetWindow());

        PhsyicalDevice = ClVkPhysicalDevice::Get();
        PhsyicalDevice->Pick();

        Device = ClVkDevice::Get();
        Device->Create();

        QueueFamilies = ClVkQueueFamilies::Get();
        QueueFamilies->SetQueues();

        MemAllocator = ClVkMemAllocator::Get();
        MemAllocator->Create();

        return true;
    }


    void ClVkContext::Destroy()
    {
        MemAllocator->Destroy();
        Device->Destroy();
        Surface->Destroy();
        ValidationLayers->Destroy();
        Instance->Destroy();

        // ClVkLoader::CloseLibrary(m_vk_lib);
    }

} // namespace Clunk
