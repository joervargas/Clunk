#pragma once

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <PCH/pch.h>


namespace Clunk
{
    class ClVkDepthImage
    {
    public:

        const VkImage* GetImage() { return &m_image; }
        const VmaAllocation* GetAllocation() { return &m_allocation; }
        const VkImageView* GetView() { return &m_imageView; }
        // const VkSampler* GetSampler() { return &m_sampler; }
        VkFormat GetFormat() { return m_format; }

        b8 HasStencilComponent() { return m_format == VK_FORMAT_D32_SFLOAT_S8_UINT || m_format == VK_FORMAT_D24_UNORM_S8_UINT; }


        void Create(const VkExtent2D& SwapchainExtent, const VkSampleCountFlagBits& MsaaSampleCount);
        void Destroy();

    private:
       
        VkImage m_image;
        VmaAllocation m_allocation;
        VkImageView m_imageView;
        // VkSampler m_sampler;
        VkFormat m_format;

        VkFormat FindFormat();
    };
}