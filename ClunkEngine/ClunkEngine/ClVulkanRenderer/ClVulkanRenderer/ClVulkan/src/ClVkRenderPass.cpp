#include "ClVkRenderPass.h"
#include "ClVkImg.h"

namespace Clunk::Vk
{
    ClVkRenderPass cl_create_vk_renderpass(const ClVkContext &VkCtx, const ClVkRenderPassInfo &Info)
    {
        if(!Info.bUseColor && !Info.bUseDepth) { return ClVkRenderPass{}; }

        const bool offscreen_internal = Info.flags & ERenderPassBit::ERPB_OFFSCREEN_INTERNAL;
        const bool first = Info.flags & ERenderPassBit::ERPB_FIRST;
        const bool last = Info.flags & ERenderPassBit::ERPB_LAST;

        std::vector<VkAttachmentDescription> attachments;

        VkAttachmentDescription color_attachment = {};
        VkAttachmentReference color_attachment_ref = {};
        if(Info.bUseColor)
        {
            color_attachment = 
            {
                .flags = 0,
                .format = Info.ColorFormat,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = offscreen_internal ? VK_ATTACHMENT_LOAD_OP_LOAD : (Info.bClearColor ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD),
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = first ? VK_IMAGE_LAYOUT_UNDEFINED : (offscreen_internal ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
                .finalLayout = last ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
            };

            if(Info.flags & ERenderPassBit::ERPB_OFFSCREEN)
            {
                if(Info.bClearColor)
                {
                    color_attachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                }
            }

            color_attachment_ref =
            {
                .attachment = 0,
                .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
            };

            attachments.push_back(color_attachment);
        }

        VkAttachmentDescription depth_attachment = {};
        VkAttachmentReference depth_attachment_ref = {};
        if(Info.bUseDepth)
        {
            depth_attachment =
            {
                .flags = 0,
                .format = find_vk_format_depth_img(VkCtx.PhysicalDevice),
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = offscreen_internal ? VK_ATTACHMENT_LOAD_OP_LOAD : (Info.bClearDepth ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD),
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = Info.bClearDepth ? VK_IMAGE_LAYOUT_UNDEFINED : (offscreen_internal ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL),
                .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
            };

            if(Info.flags & ERenderPassBit::ERPB_OFFSCREEN)
            {
                if(Info.bUseDepth)
                {
                    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                }
            }

            depth_attachment_ref =
            {
                .attachment = 1,
                .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
            };

            attachments.push_back(depth_attachment);
        }

        std::vector<VkSubpassDependency> dependencies =
        {
            VkSubpassDependency
            {
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .srcAccessMask = 0,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dependencyFlags = 0
            }
        };

        if(Info.flags & ERenderPassBit::ERPB_OFFSCREEN)
        {
            dependencies.resize(2);
            dependencies[0] =
            {
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT
            };

            dependencies[1] =
            {
                .srcSubpass = 0,
                .dstSubpass = VK_SUBPASS_EXTERNAL,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
                .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT
            };
        }

        const VkSubpassDescription subpass =
        {
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = 0,
            .pInputAttachments = nullptr,
            .colorAttachmentCount = static_cast<u32>(Info.bUseColor ? 1 : 0),
            .pColorAttachments = Info.bUseColor ? &color_attachment_ref : nullptr,
            .pResolveAttachments = nullptr,
            .pDepthStencilAttachment = Info.bUseDepth ? &depth_attachment_ref : nullptr,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr
        };

        const VkRenderPassCreateInfo create_info =
        {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = static_cast<u32>(attachments.size()),
            .pAttachments = attachments.data(),
            .subpassCount = 1,
            .pSubpasses = &subpass,
            .dependencyCount = static_cast<u32>(dependencies.size()),
            .pDependencies = dependencies.data()
        };

        ClVkRenderPass renderpass =
        {
            .Info = Info,
        };

        VK_CHECK(vkCreateRenderPass(VkCtx.Device, &create_info, nullptr, &renderpass.Handle));

        return renderpass;
    }

    void cl_destroy_vk_renderpass(const ClVkContext &VkCtx, ClVkRenderPass& RenderPass)
    {
        vkDestroyRenderPass(VkCtx.Device, RenderPass.Handle, nullptr);
    }
}