#include "ClVkEndLayer.h"



namespace Clunk::Vk
{

    ClVkEndLayer::ClVkEndLayer(ClVkContext *pVkCtx, ClVkImage* pDepthImage) :
        ClVkRenderLayerBase(pVkCtx)
    {
        ClVkRenderPassInfo renderpass_info =
        {
            .bUseColor = true,
            .bClearColor = false,
            .bUseDepth = false,
            .bClearDepth = false,
            .ColorFormat = VkFormat::VK_FORMAT_B8G8R8A8_UNORM,
            .Flags = ERenderPassBit::ERPB_NONE,
            .Samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT
        };
        mRenderPass = cl_create_vk_renderpass(*pVkCtx, renderpass_info);
        // cl_create_vk_renderpass(*pVkCtx, renderpass_info, &mRenderPass);
        
        if (pDepthImage)
        {
            mFramebuffers = cl_create_vk_color_depth_framebuffers(*pVkCtx, mRenderPass, pDepthImage->View);
        } else {
            mFramebuffers = cl_create_vk_color_only_framebuffers(*pVkCtx, mRenderPass);
        }
    }

    ClVkEndLayer::~ClVkEndLayer()
    {}

    void ClVkEndLayer::DrawFrame(const VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {
        BeginRenderPass(CmdBuffer, CurrentImage);
        EndRenderPass(CmdBuffer);
    }
}