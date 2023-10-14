#pragma once

#include <vulkan/vulkan.h>
#include <PCH/pch.h>
#include <ClVulkan/ClVkContext.h>

namespace Clunk::Vk
{
    VkDescriptorSetLayoutBinding get_vk_desc_set_layout_binding(u32 Binding, VkDescriptorType DescType, u32 Count, VkShaderStageFlags ShaderStage);

    VkWriteDescriptorSet get_vk_buffer_write_desc_set(VkDescriptorSet& DescSet, VkDescriptorBufferInfo& BufferInfo, u32 Binding, VkDescriptorType& DescType);

    VkWriteDescriptorSet get_vk_image_write_desc_set(VkDescriptorSet& DescSet, VkDescriptorImageInfo& ImageInfo, u32 Binding);

    VkDescriptorPool cl_create_vk_desc_pool(ClVkContext& VkContext, u32 UniformCount, u32 StorageCount, u32 ImgSampleCount);

    typedef struct ClVkDescriptor
    {
        std::vector<VkDescriptorSetLayout>      layouts;
        VkDescriptorPool                        pool;
        std::vector<VkDescriptorPool>           Sets;
    } ClVkDescriptor;


    void cl_destroy_vk_descriptor(ClVkContext& VkCtx, ClVkDescriptor& Descriptor);
}