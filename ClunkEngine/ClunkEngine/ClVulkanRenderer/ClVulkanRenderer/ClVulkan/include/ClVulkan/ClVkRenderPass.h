#pragma once

#include <vulkan/vulkan.h>

#include <PCH/pch.h>
#include "ClVkContext.h"


namespace Clunk::Vk
{
    enum ERenderPassBit : u8
    {
        ERPB_NONE = 0x00,
        //clear the attachment
        ERPB_FIRST = 0x01,
        // transition to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        ERPB_LAST = 0x02,
        // transition to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        ERPB_OFFSCREEN = 0x04,
        // keep VK_IMAGE_LAYOUT_*_ATTACHMENT_OPTIMAL
        ERPB_OFFSCREEN_INTERNAL = 0x08
    };

    struct ClVkRenderPassInfo
    {
        b8 bUseColor = false;
        b8 bClearColor = false;
        b8 bUseDepth = false;
        b8 bClearDepth = false;
        VkFormat ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
        u8 Flags = 0;
        VkSampleCountFlagBits Samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
    };

    struct ClVkRenderPass
    {
        ClVkRenderPassInfo Info = {};
        VkRenderPass Handle = VK_NULL_HANDLE;
    };

    ClVkRenderPass cl_create_vk_renderpass(ClVkContext& VkCtx, ClVkRenderPassInfo& Info);

    void cl_destroy_vk_renderpass(const ClVkContext& VkCtx, ClVkRenderPass& RenderPass);
}


