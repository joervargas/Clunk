#include "ClVkDescriptors.h"

#include "VkUtils.h"

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
            .pImageInfo = nullptr,
            .pBufferInfo = &BufferInfo,
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
            .pImageInfo = &ImageInfo,
            .pBufferInfo = nullptr,
            .pTexelBufferView = nullptr
        };
    }

    VkDescriptorPool cl_create_vk_desc_pool(ClVkContext &VkCtx, u32 UniformCount, u32 StorageCount, u32 ImgSampleCount)
    {
        u32 img_count = VkCtx.FrameSync.GetNumFramesInFlight();
        std::vector<VkDescriptorPoolSize> pool_sizes;

        if(UniformCount > 0)
        {
            pool_sizes.push_back(
                VkDescriptorPoolSize
                {
                    .type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    .descriptorCount = img_count * UniformCount
                }
            );
        }

        if(StorageCount > 0)
        {
            pool_sizes.push_back(
                VkDescriptorPoolSize
                {
                    .type = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                    .descriptorCount = img_count * StorageCount
                }
            );
        }

        if (ImgSampleCount > 0)
        {
            pool_sizes.push_back(
                VkDescriptorPoolSize
                {
                    .type = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
                    .descriptorCount = img_count * ImgSampleCount
                }
            );
        }
        
        VkDescriptorPoolCreateInfo create_info =
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = img_count,
            .poolSizeCount = static_cast<u32>(pool_sizes.size()),
            .pPoolSizes = pool_sizes.data()
        };

        VkDescriptorPool desc_pool;
        VK_CHECK(vkCreateDescriptorPool(VkCtx.Device, &create_info, nullptr, &desc_pool));

        return desc_pool;
    }

    void cl_destroy_vk_descriptor(const ClVkContext &VkCtx, ClVkDescriptor &Descriptor)
    {
        if(!Descriptor.Layouts.empty() && Descriptor.Layouts[0] != nullptr)
        {
            vkDestroyDescriptorSetLayout(VkCtx.Device, Descriptor.Layouts.front(), nullptr);
            Descriptor.Layouts.clear();
        }
        if(Descriptor.Pool != nullptr)
        {
            vkDestroyDescriptorPool(VkCtx.Device, Descriptor.Pool, nullptr);
            Descriptor.Sets.clear();
        }
    }
}
