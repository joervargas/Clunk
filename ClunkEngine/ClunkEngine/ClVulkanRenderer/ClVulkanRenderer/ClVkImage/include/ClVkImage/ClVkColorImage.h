#pragma once

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <PCH/pch.h>

namespace Clunk
{
    class ClVkColorImage
    {
    public:

        const VkImage* GetImage() const { return &m_image; }
        const VkImageView* GetView() const { return &m_imageView; }
        const VmaAllocation* GetAllocation() const { return &m_allocation; }

        void Create(const VkExtent2D& SwapchainExtent, const VkFormat& SwapchainFormat, const VkSampleCountFlagBits& MsaaSampleCount = VK_SAMPLE_COUNT_1_BIT);
        void Destroy();

    private:

        VkImage m_image;
        VmaAllocation m_allocation;
        VkImageView m_imageView;

    };
}