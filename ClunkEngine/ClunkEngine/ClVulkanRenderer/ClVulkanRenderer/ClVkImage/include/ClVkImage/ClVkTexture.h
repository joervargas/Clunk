#pragma once

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <PCH/pch.h>


namespace Clunk
{
    class ClVkTexture
    {
    public:

        const VkImage* GetImage() { return &m_image; }
        const VmaAllocation* GetAllocation() { return &m_allocation; }
        const VkImageView* GetView() { return &m_imageView; }
        const VkSampler* GetSampler() { return &m_sampler; }

        void FromFile(const char* FilePath);
        // void FromBuffer(void* Buffer, size_t Components, u32 Width, u32 Height);
        void Destroy();

    private:

        u32 m_mipLevels;
        u32 m_layers;
        u32 m_baseWidth;
        u32 m_baseHeight;
        
        VkImage m_image;
        VmaAllocation m_allocation;
        VkImageView m_imageView;
        VkSampler m_sampler;

        void CreateSTD(const char* FilePath, b8 bGenerateMipMaps = false);
        // void CreateKTX(const char* FilePath);

    };
}