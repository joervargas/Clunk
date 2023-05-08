#pragma once

#include <vulkan/vulkan.h>
#include <PCH/pch.h>

namespace Clunk
{
    class ClVkGraphicsPipeline
    {
    public:

        ClVkGraphicsPipeline() {};
        ~ClVkGraphicsPipeline() {};

        // void Create(const VkRenderPass& RenderPass, const VkSampleCountFlagBits& MsaaSampleCount, const VkDescriptorSetLayout* DescSetLayout);
        void Create(const VkRenderPass& RenderPass, const VkSampleCountFlagBits& MsaaSampleCount, const VkPipelineLayout* pPipelineLayout);
        void Destroy();

        const VkPipeline* Get() { return &m_pipeline; }
        const VkPipelineLayout* GetLayout() { return &m_layout; }

    private:

        VkPipeline m_pipeline;
        VkPipelineLayout m_layout;

    };
}
