#include "ClVkDevice.h"
#include "ClVkPhysicalDevice.h"
#include "ClVkQueueFamilies.h"
#include "ClVkValidationLayers.h"

#include <Core/Exception.h>
#include <Core/Logger.h>

#include <ClVkExt/ClVkExt.h>


ClVkDevice ClVkDevice::m_ClVkDevice;

// PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;

void ClVkDevice::Create()
{
    CLOG_INFO("Creating Logical Device...");

    ClVkPhysicalDevice* PhysDevice = ClVkPhysicalDevice::Get();
    
    ClVkQueueFamilies* QueueFamilies = ClVkQueueFamilies::Get();
    auto deviceQueueInfo = QueueFamilies->DeviceQueueInfo();

    // extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = PhysDevice->Features();
    createInfo.pQueueCreateInfos = deviceQueueInfo.data();
    createInfo.queueCreateInfoCount = static_cast<u32>(deviceQueueInfo.size());
    // createInfo.pEnabledFeatures = &PhysDevice->Features()->features;
    createInfo.enabledExtensionCount = static_cast<u32>(DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = DeviceExtensions.data();
    if(ENABLE_VALIDATION_LAYERS)
    {
        createInfo.enabledLayerCount = static_cast<u32>(VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    VK_CHECK(vkCreateDevice(*PhysDevice->Handle(), &createInfo, nullptr, &m_device));

    vk_ext::LoadDeviceFunctions(m_device, DeviceExtensions);
    // vkCmdPushDescriptorSetKHR = (PFN_vkCmdPushDescriptorSetKHR)vkGetDeviceProcAddr(m_device, "vkCmdPushDescriptorSetKHR");

    CLOG_INFO("Logical Device Created.");
}

void ClVkDevice::Destroy()
{
    CLOG_INFO("Destroying Vulkan Logical Device...");

    vkDestroyDevice(m_device, nullptr);

    CLOG_INFO("Vulkan Logical Device Destroyed.");
}