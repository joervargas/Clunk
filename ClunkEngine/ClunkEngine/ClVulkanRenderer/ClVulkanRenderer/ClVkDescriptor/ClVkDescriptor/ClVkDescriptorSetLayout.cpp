#include "ClVkDescriptorSetLayout.h"
#include <ClVkContext/ClVkDevice.h>

#include <Core/Exception.h>
namespace Clunk
{
    void ClVkDescriptorSetLayout::Init()
    {
        const u32 MAX_SETS = 2;

        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        // uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorCount = MAX_SETS;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        // samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorCount = MAX_SETS;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        m_bindings.push_back(uboLayoutBinding);
        m_bindings.push_back(samplerLayoutBinding);
    }

    void ClVkDescriptorSetLayout::PushBinding(const VkDescriptorSetLayoutBinding &Binding)
    {
        m_bindings.push_back(Binding);
    }

    void ClVkDescriptorSetLayout::Create()
    {
        VkDescriptorSetLayoutCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.bindingCount = static_cast<u32>(m_bindings.size());
        createInfo.pBindings = m_bindings.data();
        // createInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR;
        createInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT_EXT;

            VkDescriptorBindingFlags bindless_flags = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT |
                VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT;
            VkDescriptorSetLayoutBindingFlagsCreateInfoEXT extendedInfo
            {
                VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT, nullptr 
            };
            extendedInfo.bindingCount = static_cast<u32>(m_bindings.size());
            extendedInfo.pBindingFlags = &bindless_flags;
        
        createInfo.pNext = &extendedInfo;

        ClVkDevice* Device = ClVkDevice::Get();
        VK_CHECK(Device->CreateDescSetLayout(&createInfo, nullptr, &m_descSetLayout));
    }

    void ClVkDescriptorSetLayout::Destroy()
    {
        ClVkDevice* Device = ClVkDevice::Get();
        Device->DestroyDescSetLayout(m_descSetLayout, nullptr);
    }
}