#include "ClVkBeginLayer.h"
#include <ClVulkan/ClVkRenderPass.h>

namespace Clunk::Vk
{

    ClVkBeginLayer::ClVkBeginLayer(const ClVkContext &VkCtx, ClVkImage* pDepthImage)
    {
        ClVkRenderPassInfo renderpass_info =
        {
            .bUseColor = true,
            .bClearColor = true,
            .bUseDepth = pDepthImage != nullptr ? true : false,
            .bClearDepth = pDepthImage != nullptr ? true : false,
            // .ColorFormat = VkFormat::VK_FORMAT_B8G8R8A8_UNORM,
            .ColorFormat = VkCtx.Swapchain.Format,
            .Flags = ERenderPassBit::ERPB_FIRST,
            .Samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT
        };
        mRenderPass = Clunk::Vk::cl_create_vk_renderpass(VkCtx, renderpass_info);
        // cl_create_vk_renderpass(*pVkCtx, renderpass_info, &mRenderPass);

        if (pDepthImage)
        {
            mFramebuffers = cl_create_vk_color_depth_framebuffers(VkCtx, mRenderPass, pDepthImage->View);
        } else {
            mFramebuffers = cl_create_vk_color_only_framebuffers(VkCtx, mRenderPass);
        }
    }

    ClVkBeginLayer::~ClVkBeginLayer()
    {}

    void ClVkBeginLayer::DrawFrame(const ClVkContext& VkCtx, const VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {
        BeginRenderPass(VkCtx, CmdBuffer, CurrentImage);
        EndRenderPass(CmdBuffer);
    }
}