#include "ClVkQueueFamilies.h"
#include "ClVkDevice.h"
#include "ClVkSurface.h"

#include <Core/Logger.h>

#include <set>


ClVkQueueFamilies ClVkQueueFamilies::m_ClVkQueueFamilies;

void ClVkQueueFamilies::Find(const VkPhysicalDevice &PhysDevice)
{
    // Reset std::optional values for queue family indices
    m_indices.Reset();

    u32 queueFamilyCount{0};
    vkGetPhysicalDeviceQueueFamilyProperties(PhysDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(PhysDevice, &queueFamilyCount, queueFamiliesProperties.data());

    ClVkSurface* Surface = ClVkSurface::Get();
    i32 i{0}; // index value to increment
    for( const auto& queueFamilyProps : queueFamiliesProperties)
    {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(PhysDevice, i, *Surface->Handle(), &presentSupport);

        // Check for a dedicated Compute Queue
        if((queueFamilyProps.queueFlags & VK_QUEUE_COMPUTE_BIT) && 
            ((queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT) ==0) && 
            !presentSupport)
        {
            m_indices.compute = i;
        }
        // // Check for dedicated Transfer Queue
        // if((queueFamilyProps.queueFlags & VK_QUEUE_TRANSFER_BIT) && 
        //     ((queueFamilyProps.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0) && 
        //     ((queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT) ==0))
        // {

        // }
        // Check for a dedicated Graphics Queue
        if((queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
            ((queueFamilyProps.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0) &&
            !presentSupport)
        {
            m_indices.graphics = i;
        }
        // Check for dedicated Present Queue
        if(presentSupport &&
            ((queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) &&
            ((queueFamilyProps.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
        {
            m_indices.present = i;
        }
        i++;
    }

    if(IsComplete()) // Found dedicated queue family indices for all queues 
    {
        m_sharingmode = VK_SHARING_MODE_CONCURRENT;
    }
    else // Some indices missing. 
    {
        i = 0; // set index value back to 0
        for(const auto& queueFamilyProps : queueFamiliesProperties)
        {
            if(!m_indices.graphics.has_value() && queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_indices.graphics = i;
            }
            if(!m_indices.compute.has_value() && queueFamilyProps.queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                m_indices.compute = i;
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(PhysDevice, i, *Surface->Handle(), &presentSupport);
            if(!m_indices.present.has_value() && presentSupport)
            {
                m_indices.present = i;
            }
            i++;
        }
        m_sharingmode = VK_SHARING_MODE_EXCLUSIVE;
    }
    // SetSharingMode();
}

void ClVkQueueFamilies::SetQueues()
{
    CLOG_INFO("Setting Queue Command Handles...");

    ClVkDevice* Device = ClVkDevice::Get();

    if(m_indices.graphics.has_value())
    {
        vkGetDeviceQueue(*Device->Handle(), m_indices.graphics.value(), 0, &m_graphics);
    }

    if(m_indices.present.has_value())
    {
        vkGetDeviceQueue(*Device->Handle(), m_indices.present.value(), 0, &m_present);
    }

    if(m_indices.compute.has_value())
    {
        vkGetDeviceQueue(*Device->Handle(), m_indices.compute.value(), 0, &m_compute);
    }
    CLOG_INFO("Queue Command Handles Set.");
}

std::vector<VkDeviceQueueCreateInfo> ClVkQueueFamilies::DeviceQueueInfo()
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

    std::set<u32> uniqueQueueFamilies;
    if(m_indices.graphics.has_value())
    {
        uniqueQueueFamilies.emplace(m_indices.graphics.value());
    }
    if(m_indices.present.has_value())
    {
        uniqueQueueFamilies.emplace(m_indices.present.value());
    }
    if(m_indices.compute.has_value())
    {
        uniqueQueueFamilies.emplace(m_indices.compute.value());
    }
    // std::set<u32> uniqueQueueFamilies = {
    //     m_indices.graphics.value(),
    //     m_indices.present.value()
    // };

    for(u32 queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &m_queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    return queueCreateInfos;
}

std::vector<u32> ClVkQueueFamilies::GetIndexList()
{
    std::vector<u32> queueIndices;

    if (m_indices.graphics.has_value()) { queueIndices.push_back(m_indices.graphics.value()); }
    if (m_indices.present.has_value()) { queueIndices.push_back(m_indices.present.value()); }
    if (m_indices.compute.has_value()) { queueIndices.push_back(m_indices.compute.value()); }

    return queueIndices;
}
