#include "ClVkRenderLayers/ClVkRenderLayerBase.h"
#include "ClVkRenderLayerBase.h"


namespace Clunk::Vk
{
    void ClVkRenderLayerBase::Destroy(ClVkContext& VkCtx)
    {
        // if(mDescSetLayout != nullptr)
        // {
        //     vkDestroyDescriptorSetLayout(VkCtx.Device, mDescSetLayout, nullptr);
        // }
        // if(mDescPool != nullptr)
        // {
        //     vkDestroyDescriptorPool(VkCtx.Device, mDescPool, nullptr);
        // }
        cl_destroy_vk_descriptor(VkCtx, mDescriptor);
        
        for(VkFramebuffer frame_buffer : mFramebuffers)
        {
            if(frame_buffer != nullptr)
            {
                vkDestroyFramebuffer(VkCtx.Device, frame_buffer, nullptr);
            }
        }
        if(mRenderPass.Handle != nullptr)
        {
            vkDestroyRenderPass(VkCtx.Device, mRenderPass.Handle, nullptr);
        }
        if(mPipelineLayout != nullptr)
        {
            vkDestroyPipelineLayout(VkCtx.Device, mPipelineLayout, nullptr);
        }
        if (mPipeline != nullptr)
        {
            vkDestroyPipeline(VkCtx.Device, mPipeline, nullptr);
        }
    }

    ClVkRenderLayerBase::~ClVkRenderLayerBase() {}

    void ClVkRenderLayerBase::BeginRenderPass(const ClVkContext& VkCtx, const VkCommandBuffer& CmdBuffer, u32 CurrentImage)
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
                .width = VkCtx.Swapchain.Width,
                .height = VkCtx.Swapchain.Height
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
        if(mPipeline != VK_NULL_HANDLE)
        {
            vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);
        }

        VkViewport viewport =
        {
            .x = 0.0f, 
            .y = 0.0f,
            .width = static_cast<f32>(VkCtx.Swapchain.Width),
            .height = static_cast<f32>(VkCtx.Swapchain.Height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        VkRect2D scissor =
        {
            .offset = VkOffset2D{ .x = 0, .y = 0},
            .extent = VkExtent2D {
                .width = VkCtx.Swapchain.Width,
                .height = VkCtx.Swapchain.Height
            }
        };

        vkCmdSetViewport(CmdBuffer, 0, 1, &viewport);
        vkCmdSetScissor(CmdBuffer, 0, 1, &scissor);
    }

    void ClVkRenderLayerBase::EndRenderPass(const VkCommandBuffer& CmdBuffer)
    {
        vkCmdEndRenderPass(CmdBuffer);
    }

    void ClVk2dLayerList::Destroy(ClVkContext& VkCtx)
    {
        for(auto& layer : mList)
        {
            layer->Destroy(VkCtx);
        }
    }

    void ClVk2dLayerList::Update(ClVkContext& VkCtx, u32 CurrentIndex, f32 DeltaTime)
    {
        for(auto& layer : mList)
        {
            layer->Update(VkCtx, CurrentIndex, DeltaTime);
        }
        // for(u32 i = 0; i < mList.size(); i++)
        // {
        //     mList[i]->Update(CurrentIndex, DeltaTime);
        // }
    }
    void ClVk2dLayerList::DrawFrame(const ClVkContext& VkCtx, const VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {
        for(auto& layer : mList)
        {
            layer->DrawFrame(VkCtx, CmdBuffer, CurrentImage);
        }
    }

    void ClVk3dLayerList::Destroy(ClVkContext& VkCtx)
    {
        for(auto& layer : mList)
        {
            layer->Destroy(VkCtx);
        }
    }

    void ClVk3dLayerList::Update(ClVkContext& VkCtx, u32 CurrentIndex, ClVkBuffer &TransformUniform, const ClVkTransforms &Transforms, f32 DeltaTime)
    {
        for(auto& layer : mList)
        {
            layer->Update(VkCtx, CurrentIndex, TransformUniform, Transforms, DeltaTime);
        }
    }

    void ClVk3dLayerList::DrawFrame(const ClVkContext& VkCtx, const VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {
        for(auto& layer : mList)
        {
            layer->DrawFrame(VkCtx, CmdBuffer, CurrentImage);
        }
    }
}
