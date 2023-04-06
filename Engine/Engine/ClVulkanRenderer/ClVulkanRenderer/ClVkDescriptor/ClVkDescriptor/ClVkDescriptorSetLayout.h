#pragma once

#include <vulkan/vulkan.h>
#include <Defines/Defines.h>
#include <PCH/pch.h>


namespace Clunk
{
    class ClVkDescriptorSetLayout
    {
    public:

        VkDescriptorSetLayout* Handle() { return &m_descSetLayout; }

        void Init();
        void PushBinding(const VkDescriptorSetLayoutBinding& Binding);
        void Create();
        void Destroy();

    private:

        std::vector<VkDescriptorSetLayoutBinding> m_bindings;
        VkDescriptorSetLayout m_descSetLayout;
    
    };
}