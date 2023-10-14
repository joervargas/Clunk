#include "ClVkDescriptors.h"


namespace Clunk::Vk
{
    VkDescriptorSetLayoutBinding get_vk_desc_set_layout_binding(u32 Binding, VkDescriptorType DescType, u32 Count, VkShaderStageFlags ShaderStage)
    {
        return VkDescriptorSetLayoutBinding
        {
            .binding = Binding,
            .descriptorType = DescType,
            .descriptorCount = Count,
            .stageFlags = ShaderStage,
            .pImmutableSamplers = nullptr
        };
    }

    VkWriteDescriptorSet get_vk_buffer_write_desc_set(VkDescriptorSet &DescSet, VkDescriptorBufferInfo &BufferInfo, u32 Binding, VkDescriptorType &DescType)
    {
        return VkWriteDescriptorSet
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = DescSet,
            .dstBinding = Binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = DescType,
            .pBufferInfo = &BufferInfo,
            .pImageInfo = nullptr,
            .pTexelBufferView = nullptr
        };
    }

    VkWriteDescriptorSet get_vk_image_write_desc_set(VkDescriptorSet &DescSet, VkDescriptorImageInfo &ImageInfo, u32 Binding)
    {
        return VkWriteDescriptorSet
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = DescSet,
            .dstBinding = Binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pBufferInfo = nullptr,
            .pImageInfo = &ImageInfo,
            .pTexelBufferView = nullptr
        };
    }

    VkDescriptorPool cl_create_vk_desc_pool(ClVkContext &VkContext, u32 UniformCount, u32 StorageCount, u32 ImgSampleCount)
    {
        return VkDescriptorPool();
    }

    void cl_destroy_vk_descriptor(ClVkContext &VkCtx, ClVkDescriptor &Descriptor)
    {
    }
}
