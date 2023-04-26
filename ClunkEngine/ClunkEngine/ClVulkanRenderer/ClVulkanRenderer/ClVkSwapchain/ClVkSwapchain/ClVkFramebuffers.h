#pragma once

#include <ClVkContext/ClVkDevice.h>
#include <vulkan/vulkan.h>

#include <Defines/Defines.h>
#include <PCH/pch.h>


namespace Clunk
{
    class ClVkFramebuffers
    {
    public:

        void Create(
            const std::vector<VkImageView>& SwapchainImageViews,
            const VkImageView& DepthImageView,
            const VkImageView& ColorImageMSAAView,
            const VkExtent2D& SwapchainExtent,
            const VkRenderPass& RenderPass
        );

        void Destroy();

        VkFramebuffer& operator[](size_t Index) { return m_framebuffers[Index]; }

        u32 Size() { return static_cast<u32>(m_framebuffers.size()); }

    private:

        std::vector<VkFramebuffer> m_framebuffers;

    };
}