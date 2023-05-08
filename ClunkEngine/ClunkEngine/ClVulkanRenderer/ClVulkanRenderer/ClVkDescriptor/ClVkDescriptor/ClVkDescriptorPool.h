#pragma once

#include <vulkan/vulkan.h>
#include <PCH/pch.h>

namespace Clunk
{
    class ClVkDescriptorPool
    {
    public:

        // void Init();
        void Create();
        void Destroy();

        VkDescriptorPool* Handle() { return &m_descPool; }

    private:

        u32 m_MaxCount{0};
        VkDescriptorPoolSize m_UnfiformPoolSize;
        VkDescriptorPoolSize m_SamplerPoolSize;

        VkDescriptorPool m_descPool;
    };
}