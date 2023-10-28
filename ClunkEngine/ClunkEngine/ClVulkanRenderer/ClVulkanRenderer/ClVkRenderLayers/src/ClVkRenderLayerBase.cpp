#include "ClVkRenderLayers/ClVkRenderLayerBase.h"
#include "ClVkRenderLayerBase.h"


namespace Clunk::Vk
{
    void ClVkRenderLayerBase::Destroy()
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
            if(frame_buffer != nullptr)
            {
                vkDestroyFramebuffer(pVkCtx->Device, frame_buffer, nullptr);
            }
        }
        if(mRenderPass.Handle != nullptr)
        {
            vkDestroyRenderPass(pVkCtx->Device, mRenderPass.Handle, nullptr);
        }
        if(mPipelineLayout != nullptr)
        {
            vkDestroyPipelineLayout(pVkCtx->Device, mPipelineLayout, nullptr);
        }
        if(mPipeline != nullptr)
        {
            vkDestroyPipeline(pVkCtx->Device, mPipeline, nullptr);
        }
    }

    ClVkRenderLayerBase::~ClVkRenderLayerBase()
    {
    }

    void ClVkRenderLayerBase::BeginRenderPass(const VkCommandBuffer& CmdBuffer, u32 CurrentImage)
    {
        std::vector<VkClearValue> clear_values;
        if (mRenderPass.Info.bClearColor) 
        {
            clear_values.push_back(
                VkClearValue
                {
                    .color = VkClearColorValue{ .float32 = { 0.0f, 0.0f, 0.0f, 1.0f } }
                }
            );
        }
        if (mRenderPass.Info.bClearDepth)
        {
            clear_values.push_back(
                VkClearValue
                {
                    .depthStencil = VkClearDepthStencilValue{ .depth = 1.0, .stencil = 0 }
                }
            );
        }

        VkRect2D screen_rect =
        {
            .offset = VkOffset2D{ .x = 0, .y = 0 },
            .extent = VkExtent2D{
                .width = pVkCtx->Swapchain.Width,
                .height = pVkCtx->Swapchain.Height
            }
        };

        VkRenderPassBeginInfo render_begin_info =
        {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = mRenderPass.Handle,
            .framebuffer = mFramebuffers[CurrentImage],
            .renderArea = screen_rect,
            .clearValueCount = static_cast<u32>(clear_values.size()),
            .pClearValues = clear_values.data()
        };
        vkCmdBeginRenderPass(CmdBuffer, &render_begin_info, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);

        VkViewport viewport =
        {
            .x = 0.0f, 
            .y = 0.0f,
            .width = static_cast<f32>(pVkCtx->Swapchain.Width),
            .height = static_cast<f32>(pVkCtx->Swapchain.Height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        VkRect2D scissor =
        {
            .offset = VkOffset2D{ .x = 0, .y = 0},
            .extent = VkExtent2D {
                .width = pVkCtx->Swapchain.Width,
                .height = pVkCtx->Swapchain.Height
            }
        };

        vkCmdSetViewport(CmdBuffer, 0, 1, &viewport);
        vkCmdSetScissor(CmdBuffer, 0, 1, &scissor);
    }

    void ClVkRenderLayerBase::EndRenderPass(const VkCommandBuffer& CmdBuffer)
    {
        vkCmdEndRenderPass(CmdBuffer);
    }

    void ClVk2dLayerList::Destroy()
    {
        for(ClVk2dLayer* layer : mList)
        {
            layer->Destroy();
        }
    }

    void ClVk2dLayerList::Update(u32 CurrentIndex, f32 DeltaTime)
    {
        for(ClVk2dLayer* layer : mList)
        {
            layer->Update(CurrentIndex, DeltaTime);
        }
    }

    void ClVk2dLayerList::DrawFrame(const VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {
        for(ClVk2dLayer* layer : mList)
        {
            layer->DrawFrame(CmdBuffer, CurrentImage);
        }
    }

    void ClVk3dLayerList::Destroy()
    {
        for(ClVk3dLayer* layer : mList)
        {
            layer->Destroy();
        }
    }

    void ClVk3dLayerList::Update(u32 CurrentIndex, ClVkBuffer &TransformUniform, ClVkImage &DepthImage, f32 DeltaTime)
    {
        for(ClVk3dLayer* layer : mList)
        {
            layer->Update(CurrentIndex, TransformUniform, DepthImage, DeltaTime);
        }
    }

    void ClVk3dLayerList::DrawFrame(const VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {
        for(ClVk3dLayer* layer : mList)
        {
            layer->DrawFrame(CmdBuffer, CurrentImage);
        }
    }
}
