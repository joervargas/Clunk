#include "ClVkDescriptorPool.h"

#include <ClVkContext/ClVkDevice.h>
#include <Core/Exception.h>

#include <array>

namespace Clunk
{

    void ClVkDescriptorPool::Create()
    {
        // TODO: Stop hard coding the descriptorCount
        const u32 MAX_SETS = 2;

        m_UnfiformPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        m_UnfiformPoolSize.descriptorCount = MAX_SETS;

        m_SamplerPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        m_SamplerPoolSize.descriptorCount = MAX_SETS;

        std::array<VkDescriptorPoolSize, 2> poolSizes[] = { 
            m_UnfiformPoolSize,
            m_SamplerPoolSize
        };

        // std::array<VkDescriptorPoolSize, 3> pool_sizes_bindless[] =
        // {
        //     { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_SETS },
        //     { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_SETS }
        //     // { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, MAX_SETS }  
        // };

        VkDescriptorPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        // createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;
        createInfo.poolSizeCount = static_cast<u32>(poolSizes->size());
        createInfo.pPoolSizes = poolSizes->data();
        createInfo.maxSets = MAX_SETS;
        // createInfo.poolSizeCount = static_cast<u32>(pool_sizes_bindless->size());
        // createInfo.pPoolSizes = pool_sizes_bindless->data();
        // createInfo.maxSets = MAX_SETS * pool_sizes_bindless->size();

        ClVkDevice* Device = ClVkDevice::Get();
        VK_CHECK(Device->CreateDescPool(&createInfo, nullptr, &m_descPool));
    }

    void ClVkDescriptorPool::Destroy()
    {
        ClVkDevice* Device = ClVkDevice::Get();
        Device->DestroyDescPool(m_descPool, nullptr);
    }
}