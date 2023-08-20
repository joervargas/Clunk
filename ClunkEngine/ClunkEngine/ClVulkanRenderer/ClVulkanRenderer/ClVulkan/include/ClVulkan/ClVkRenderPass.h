#pragma once

#include <vulkan/vulkan.h>

#include <PCH/pch.h>
#include "ClVkContext.h"


namespace Clunk::Vk
{
    enum ERenderPassBit : u8
    {
        //clear the attachment
        ERPB_FIRST = 0x01,
        // transition to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        ERPB_LAST = 0x02,
        // transition to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        ERPB_OFFSCREEN = 0x04,
        // keep VK_IMAGE_LAYOUT_*_ATTACHMENT_OPTIMAL
        ERPB_OFFSCREEN_INTERNAL = 0x08
    };

    struct ClVkRenderPassInfo final
    {
        b8 bUseColor = false;
        b8 bClearColor = false;
        VkFormat ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
        b8 bUseDepth = false;
        b8 bClearDepth = false;
        u8 flags = 0;
    };

    struct ClVkRenderPass
    {
        const ClVkRenderPassInfo Info = {};
        VkRenderPass Handle = VK_NULL_HANDLE;
    };

    ClVkRenderPass cl_create_vk_renderpass(const ClVkContext& VkCtx, const ClVkRenderPassInfo& Info = {});

}


