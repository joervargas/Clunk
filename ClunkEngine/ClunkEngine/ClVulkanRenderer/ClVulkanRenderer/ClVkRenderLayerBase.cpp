#include "ClVkRenderLayerBase.h"


namespace Clunk::Vk
{
    
    ClVkRenderLayerBase::~ClVkRenderLayerBase()
    {
        for(VkBuffer buf: mUniformBuffers)
        {
            vkDestroyBuffer(pVkCtx->Device, buf, nullptr);
        }
        for(VkDeviceMemory mem: mUniformBuffersDeviceMemory)
        {
            vkFreeMemory(pVkCtx->Device, mem, nullptr);
        }
        if(mDescriptorSetLayout != nullptr)
        {
            vkDestroyDescriptorSetLayout(pVkCtx->Device, mDescriptorSetLayout, nullptr);
        }
        if(mDescriptorPool != nullptr)
        {
            vkDestroyDescriptorPool(pVkCtx->Device, mDescriptorPool, nullptr);
        }
        for(VkFramebuffer frame_buffer : mFramebuffers)
        {
            vkDestroyFramebuffer(pVkCtx->Device, frame_buffer, nullptr);
        }
        vkDestroyRenderPass(pVkCtx->Device, mRenderPass.Handle, nullptr);
        vkDestroyPipelineLayout(pVkCtx->Device, mPipelineLayout, nullptr);
        vkDestroyPipeline(pVkCtx->Device, mPipeline, nullptr);
    }

    void ClVkRenderLayerBase::BeginRenderPass(VkCommandBuffer CmdBuf, u32 CurrentImage)
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
        vkCmdBeginRenderPass(CmdBuf, &begin_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(CmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);
        vkCmdBindDescriptorSets(
            CmdBuf,
            VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout,
            0, 1, &mDescriptorSets[CurrentImage],
            0, nullptr
        );
    }
}
