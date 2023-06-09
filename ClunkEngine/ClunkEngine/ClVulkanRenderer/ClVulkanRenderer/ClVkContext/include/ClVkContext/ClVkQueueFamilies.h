#pragma once

#include <vulkan/vulkan.h>
#include <PCH/pch.h>

#include <optional>


class ClVkQueueFamilies
{
private:

    struct QueueIndices
    {
        std::optional<u32> graphics;
        std::optional<u32> present;
        // std::optional<u32> transfer;
        std::optional<u32> compute;

        void Reset()
        {
            graphics.reset();
            present.reset();
            compute.reset();
        }
    } m_indices;

public:

    ~ClVkQueueFamilies() {};

    static ClVkQueueFamilies* Get() { return &m_ClVkQueueFamilies; }

    const VkQueue* Graphics() { return &m_graphics; }
    const VkQueue* Present() { return &m_present; }
    // const VkQueue* Transfer() { return &m_transfer; }
    const VkQueue* Compute() { return &m_compute; }

    const QueueIndices* Indices() { return &m_indices; }
    
    b8 IsComplete() 
    { 
        return m_indices.graphics.has_value() && 
            m_indices.present.has_value() && 
            m_indices.compute.has_value();
    }

    void Find(const VkPhysicalDevice& PhysDevice);

    void SetQueues();

    std::vector<VkDeviceQueueCreateInfo> DeviceQueueInfo();

    std::vector<u32> GetIndexList();

    VkSharingMode GetSharingMode() { return m_sharingmode; }

private:

    ClVkQueueFamilies() {};

    VkQueue m_graphics;
    VkQueue m_present;
    // VkQueue m_transfer;
    VkQueue m_compute;

    VkSharingMode m_sharingmode = VK_SHARING_MODE_EXCLUSIVE;

    f32 m_queuePriority{1.0f};

    static ClVkQueueFamilies m_ClVkQueueFamilies;
};