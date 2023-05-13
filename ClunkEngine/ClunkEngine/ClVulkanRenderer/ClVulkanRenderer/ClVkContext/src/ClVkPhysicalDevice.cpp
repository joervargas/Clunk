#include "ClVkPhysicalDevice.h"
#include "ClVkInstance.h"
#include "ClVkSurface.h"
#include "ClVkQueueFamilies.h"
#include "ClVkDevice.h"

#include <Core/Exception.h>
#include <Core/Logger.h>
#include <map>
#include <set>

#include <ClVkExt/ClVkExt.h>

ClVkPhysicalDevice ClVkPhysicalDevice::m_ClVkPhysicalDevice;

void ClVkPhysicalDevice::Pick()
{
    CLOG_INFO("Picking Physical Device (GPU)...")

    ClVkInstance* Instance = ClVkInstance::Get();
    if(!Instance) { THROW_EXCEPTION("Vulkan failed to retrieve Instance while picking GPU Device."); }
    
    // vkEnumeratePhysicalDevices(*Instance->Get(), &m_deviceCount, nullptr);
    Instance->EnumeratePhysDevices(&m_deviceCount, nullptr);
    if(m_deviceCount == 0)
    {
        THROW_EXCEPTION("Vulkan Physical Device: Failed to find supported GPUs");
    }

    m_physicalDeviceList.resize(m_deviceCount);
    // vkEnumeratePhysicalDevices(*Instance->Get(), &m_deviceCount, m_physicalDeviceList.data());
    Instance->EnumeratePhysDevices(&m_deviceCount, m_physicalDeviceList.data());

    std::multimap<i32, VkPhysicalDevice> candidates;
    i32 highestScore{0};
    for(const auto& physDevice : m_physicalDeviceList)
    {
        if(IsDeviceSuitable(physDevice))
        {
            i32 score = RateDeviceSuitability(physDevice);
            if(score > highestScore) 
            { 
                highestScore = score;
            }
            candidates.insert(std::make_pair(score, physDevice));
        }
    }
    if(candidates.size() > 0)
    {
        m_physicalDevice = candidates.find(highestScore)->second;
        SetPropertiesAndFeatures(m_physicalDevice);
        ClVkQueueFamilies* QueueFamilies = ClVkQueueFamilies::Get();
        QueueFamilies->Find(m_physicalDevice);
    }
    else
    {
        THROW_EXCEPTION("ClVkContext Failed to find a suitable GPU.");
    }

    CLOG_INFO("Physical Device (GPU) Picked: %s", m_physicalDeviceProperties.properties.deviceName );
}

b8 ClVkPhysicalDevice::IsDeviceSuitable(const VkPhysicalDevice &PhysDevice)
{
    ClVkQueueFamilies* QueueFamilies = ClVkQueueFamilies::Get();
    QueueFamilies->Find(PhysDevice);

    VkPhysicalDeviceFeatures2 physDeviceFeatures;
    physDeviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    physDeviceFeatures.pNext = nullptr;
    // VkPhysicalDeviceDescriptorIndexingFeaturesEXT descriptorIndexing{};
    // physDeviceFeatures.pNext = &descriptorIndexing;
    vk_ext::vkGetPhysicalDeviceFeatures2KHR(PhysDevice, &physDeviceFeatures);

    // b8 bindless_supported = descriptorIndexing.descriptorBindingPartiallyBound &&
    //     descriptorIndexing.runtimeDescriptorArray;
    VkPhysicalDevicePushDescriptorPropertiesKHR pushDescriptorProperties{};
    pushDescriptorProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR;

    VkPhysicalDeviceProperties2KHR physicalDeviceProperties{};
    physicalDeviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    physicalDeviceProperties.pNext = &pushDescriptorProperties;

    vk_ext::vkGetPhysicalDeviceProperties2KHR(PhysDevice, &physicalDeviceProperties);

    b8 bPushDescriptorProperties = pushDescriptorProperties.maxPushDescriptors > 0;
        
    ClVkSurface* Surface = ClVkSurface::Get();
    Surface->QueryDetails(PhysDevice);

    b8 bSwapChainAdequate = !Surface->details.formats.empty() && !Surface->details.presentModes.empty();

    b8 bIsSuitable = bSwapChainAdequate &&
        physDeviceFeatures.features.samplerAnisotropy &&
        CheckDeviceExtensionSupport(PhysDevice) &&
        QueueFamilies->IsComplete() &&
        bPushDescriptorProperties;

    return bIsSuitable;
}

u32 ClVkPhysicalDevice::RateDeviceSuitability(const VkPhysicalDevice &PhysDevice)
{
    VkPhysicalDeviceProperties physDeviceProperties;
    vkGetPhysicalDeviceProperties(PhysDevice, &physDeviceProperties);
    
    u32 score{0};
    if(physDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
    }
    score += physDeviceProperties.limits.maxImageDimension2D;
    
    CLOG_INFO("Physical Device (GPU) id: %d, name: %s, score: %d", physDeviceProperties.deviceID, physDeviceProperties.deviceName, score);

    return score;
}

b8 ClVkPhysicalDevice::CheckDeviceExtensionSupport(const VkPhysicalDevice &PhysDevice)
{
    u32 extensionCount{0};
    vkEnumerateDeviceExtensionProperties(PhysDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(PhysDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<String> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());
    for(const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}

void ClVkPhysicalDevice::SetPropertiesAndFeatures(const VkPhysicalDevice &PhysDevice)
{
    VkPhysicalDevicePushDescriptorPropertiesKHR pushDescriptorProperties{};
    pushDescriptorProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR;

    m_physicalDeviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    m_physicalDeviceProperties.pNext = &pushDescriptorProperties;

    vk_ext::vkGetPhysicalDeviceProperties2KHR(PhysDevice, &m_physicalDeviceProperties);

    // VkPhysicalDeviceDescriptorIndexingFeaturesEXT descriptorIndexingFeatures{};
    // m_descriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;

    m_physicalDeviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    // m_physicalDeviceFeatures.pNext = &m_descriptorIndexingFeatures;
    vk_ext::vkGetPhysicalDeviceFeatures2KHR(PhysDevice, &m_physicalDeviceFeatures);

    m_physicalDeviceMemoryProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
    vk_ext::vkGetPhysicalDeviceMemoryProperties2KHR(PhysDevice, &m_physicalDeviceMemoryProperties);
    // vkGetPhysicalDeviceProperties(PhysDevice, &m_physicalDeviceProperties);
    // vkGetPhysicalDeviceFeatures(PhysDevice, &m_physicalDeviceFeatures);
    // vkGetPhysicalDeviceMemoryProperties(PhysDevice, &m_physicalDeviceMemoryProperties);
}

