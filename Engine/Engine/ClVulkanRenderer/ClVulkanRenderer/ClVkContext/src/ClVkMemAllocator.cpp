#include "ClVkMemAllocator.h"
#include "ClVkInstance.h"
#include "ClVkPhysicalDevice.h"
#include "ClVkDevice.h"

#include <vulkan/vulkan.h>

#include <Core/Logger.h>
#include <Core/Exception.h>

ClVkMemAllocator ClVkMemAllocator::m_ClVkMemAllocator;

void ClVkMemAllocator::Create()
{
    CLOG_INFO("Creating Vk Memory Allocator...");

    ClVkInstance* Instance = ClVkInstance::Get();
    if(!Instance) { THROW_EXCEPTION("ClVkInstance* is null!"); }

    ClVkPhysicalDevice* PhysDevice = ClVkPhysicalDevice::Get();
    if(!PhysDevice) { THROW_EXCEPTION("ClVkPhysDevice* is null!"); }

    ClVkDevice* Device = ClVkDevice::Get();
    if(!Device) { THROW_EXCEPTION("ClVkDevice* is null!"); }

    VmaAllocatorCreateInfo createInfo{};
    createInfo.vulkanApiVersion = Instance->GetVersion();
    createInfo.instance = *Instance->Handle();
    createInfo.physicalDevice = *PhysDevice->Handle();
    createInfo.device = *Device->Handle();

    VK_CHECK(vmaCreateAllocator(&createInfo, &m_allocator));

    CLOG_INFO("Vk Memory Allocator created.");
}

void ClVkMemAllocator::Destroy()
{
    CLOG_INFO("Destroying Vk Memory Allocator...");
    vmaDestroyAllocator(m_allocator);
    CLOG_INFO("Vk Memory Allocator destroyed.")
}
