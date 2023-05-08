#pragma once

#include <vulkan/vulkan.h>
#include <PCH/pch.h>

#include <ClVkBuffer/ClVkUniformBuffer.h>

namespace Clunk
{
    class ClVkDescriptorSets
    {
    public:

        void Create(
            const VkDescriptorSetLayout& DescSetLayout, 
            const VkDescriptorPool& DescPool, 
            std::vector<VkBuffer> UBO,
            const VkImageView& ImageView,
            const VkSampler& ImageSampler
        );
        void Destroy();

        VkDescriptorSet* Get(u32 Index) { return &m_sets[Index]; }

    private:

        std::vector<VkDescriptorSet> m_sets;

    };
}