#include "ClVkDescriptorSet.h"
#include <ClVkContext/ClVkDevice.h>

#include "ClVkTransforms.h"
#include <Core/Exception.h>

#include <array>

namespace Clunk
{

    void ClVkDescriptorSets::Create(
            const VkDescriptorSetLayout& DescSetLayout, 
            const VkDescriptorPool& DescPool, 
            std::vector<VkBuffer> UBO,
            const VkImageView& ImageView,
            const VkSampler& ImageSampler
        )
    {
        // TODO: stop hard coding max frames in flight
        const u32 MAX_SETS = 2;
        std::vector<VkDescriptorSetLayout> layouts(MAX_SETS, DescSetLayout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = DescPool;
        allocInfo.descriptorSetCount = MAX_SETS;
        allocInfo.pSetLayouts = layouts.data();
    
        m_sets.resize(MAX_SETS);
        ClVkDevice* Device = ClVkDevice::Get();
        VK_CHECK(Device->AllocateDescSets(&allocInfo, m_sets.data()));

        for(size_t i{0}; i < MAX_SETS; i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = UBO[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(ClVkTransforms);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = ImageView;
            imageInfo.sampler = ImageSampler;

            std::array<VkWriteDescriptorSet, 2> descWrites{};
            descWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descWrites[0].dstSet = m_sets[i];
            descWrites[0].dstBinding = 0;
            descWrites[0].dstArrayElement = 0;
            descWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descWrites[0].descriptorCount = 1;
            descWrites[0].pBufferInfo = &bufferInfo;
            descWrites[0].pImageInfo = nullptr;
            descWrites[0].pTexelBufferView = nullptr;

            descWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descWrites[1].dstSet = m_sets[i];
            descWrites[1].dstBinding = 1;
            descWrites[1].dstArrayElement = 0;
            descWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descWrites[1].descriptorCount = 1;
            descWrites[1].pBufferInfo = nullptr;
            descWrites[1].pImageInfo = &imageInfo;
            descWrites[1].pTexelBufferView = nullptr;

            Device->UpdateDescSets(static_cast<u32>(descWrites.size()), descWrites.data(), 0, nullptr);
        }
    }

}