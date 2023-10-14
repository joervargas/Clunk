#include "ClVkRenderLayers/ClVkRenderLayerBase.h"
#include "ClVkRenderLayerBase.h"


namespace Clunk::Vk
{
    
    ClVkRenderLayerBase::~ClVkRenderLayerBase()
    {
        if(mDescSetLayout != nullptr)
        {
            vkDestroyDescriptorSetLayout(pVkCtx->Device, mDescSetLayout, nullptr);
        }
        if(mDescPool != nullptr)
        {
            vkDestroyDescriptorPool(pVkCtx->Device, mDescPool, nullptr);
        }
        for(VkFramebuffer frame_buffer : mFramebuffers)
        {
            vkDestroyFramebuffer(pVkCtx->Device, frame_buffer, nullptr);
        }
        vkDestroyRenderPass(pVkCtx->Device, mRenderPass.Handle, nullptr);
        vkDestroyPipelineLayout(pVkCtx->Device, mPipelineLayout, nullptr);
        vkDestroyPipeline(pVkCtx->Device, mPipeline, nullptr);
    }

    void ClVkRenderLayerBase::BeginRenderPass(VkCommandBuffer CmdBuffer, u32 CurrentImage)
    {
        const VkRect2D screen_rect =
        {
            .offset = { 0, 0 },
            .extent =
            {
                .width = pVkCtx->Swapchain.Width,
                .height = pVkCtx->Swapchain.Height,
            }
        };

        const VkRenderPassBeginInfo begin_info =
        {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = mRenderPass.Handle,
            .framebuffer = mFramebuffers[CurrentImage],
            .renderArea = screen_rect
        };
        vkCmdBeginRenderPass(CmdBuffer, &begin_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);
        vkCmdBindDescriptorSets(
            CmdBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout,
            0, 1, &mDescSets[CurrentImage],
            0, nullptr
        );
    }
}

void Clunk::Vk::ClVkRenderLayerBase::EndRenderPass(VkCommandBuffer CmdBuffer)
{
    vkCmdEndRenderPass(CmdBuffer);
}
