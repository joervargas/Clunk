#include "ClVkFramebuffers.h"

#include <ClVkContext/ClVkDevice.h>
#include <Core/Logger.h>
#include <Core/Exception.h>

#include <array>

namespace Clunk
{
    void ClVkFramebuffers::Create(
            const std::vector<VkImageView>& SwapchainImageViews,
            const VkImageView& DepthImageView,
            const VkImageView& ColorImageMSAAView,
            const VkExtent2D& SwapchainExtent,
            const VkRenderPass& RenderPass
        )
    {
        CLOG_INFO("Creating Vk Framebuffers...");

        size_t framebuffer_size = SwapchainImageViews.size();
        m_framebuffers.resize(framebuffer_size);

        for(size_t i = 0; i < framebuffer_size; i++)
        {
            std::array<VkImageView, 3> attachments = {
                ColorImageMSAAView,
                DepthImageView,
                SwapchainImageViews[i],
            };

            VkFramebufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.renderPass = RenderPass;
            createInfo.attachmentCount = static_cast<u32>(attachments.size());
            createInfo.pAttachments = attachments.data();
            createInfo.width = SwapchainExtent.width;
            createInfo.height = SwapchainExtent.height;
            createInfo.layers = 1;

            ClVkDevice* Device = ClVkDevice::Get();
            VK_CHECK(Device->CreateFramebuffer(&createInfo, nullptr, &m_framebuffers[i]));
        }

        CLOG_INFO("Vk Framebuffers Created.");
    }

    void ClVkFramebuffers::Destroy()
    {
        CLOG_INFO("Destroying Vk Framebuffers...");

        ClVkDevice* Device = ClVkDevice::Get();
        for(VkFramebuffer framebuffer : m_framebuffers)
        {
            Device->DestroyFramebuffer(framebuffer, nullptr);
        }

        CLOG_INFO("Vk Framebuffers Destroyed.");
    }
}