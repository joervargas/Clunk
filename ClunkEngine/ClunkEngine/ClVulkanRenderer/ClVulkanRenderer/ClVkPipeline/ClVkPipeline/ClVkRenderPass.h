#pragma once

#include <vulkan/vulkan.h>
#include <PCH/pch.h>

namespace Clunk
{
    class ClVKRenderPass
    {
    public:

        const VkRenderPass* Handle() { return &m_render_pass; }
        b8 IsInitialized() const { return bIsInitialized; }

        ClVKRenderPass() {};
        ~ClVKRenderPass() {};

        void Create(const VkFormat& SwapchainFormat, const VkSampleCountFlagBits& MsaaSampleCount, const VkFormat& DepthImageFormat);

        void Destroy();

        void Begin(const VkCommandBuffer& DrawBuffer, const VkFramebuffer& Framebuffer, const VkExtent2D& Extent);
        void End(const VkCommandBuffer& DrawBuffer);

    
    private:

        VkRenderPass m_render_pass;
        b8 bIsInitialized{false};
    };
}