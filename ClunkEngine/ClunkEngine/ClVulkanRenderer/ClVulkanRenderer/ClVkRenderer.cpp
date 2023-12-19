#include "ClVkRenderer.h"

#include <ClVulkan/ClVkLoader.h>
#include <ClVulkan/ClVkContext.h>

#include <glslang/Include/glslang_c_interface.h>


namespace Clunk::Vk
{
    ClVkRenderer::ClVkRenderer(const char* AppName, const u32 AppVersion)
    {
        glslang_initialize_process();

        ClPlatform* platform = ClPlatform::Get();
        mVkLoader = cl_create_vk_loader(platform->GetWindow(), AppName, AppVersion);
        i32 width, height;
        platform->GetDrawableSize(&width, &height);
        mVkCtx = cl_create_vk_context(mVkLoader, width, height);

        // Create a depth img
        ClVkImage depthImage = cl_create_vk_depth_image(mVkCtx, width, height);

        // Create world transform uniforms
        mWorldTransform = ClVkTransforms{
            .model = Mat4::Identity(),
            .view = Mat4::LookAtLH(Vec3(2.0f, 2.0f, 2.0f), Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 1.0f)),
            .proj = Mat4::PerspectiveLH(45.f, static_cast<f32>(width/ height), 0.1f, 100.f)
        };
        mWorldTransform.proj[5] *= -1;
        mWorldTransformUniform = cl_create_vk_gpu_buffer<ClVkTransforms>(mVkCtx, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, mWorldTransform);

        mBeginLayer = ClVkBeginLayer(mVkCtx, nullptr);
        mEndLayer = ClVkEndLayer(mVkCtx, nullptr);

        // mLayers3d = ClVk3dLayerList(&mVkCtx);

        mLayers2d.Push( new ClVkSimple2dLayer(mVkCtx, "./Assets/Images/statue.jpg") );

    }

    void ClVkRenderer::Init()
    {

    }

    void ClVkRenderer::Update(f32 DeltaTime)
    {
        // mLayers3d.Update(mVkCtx.FrameSync.GetCurrentIndex(), )
        mLayers2d.Update(mVkCtx.FrameSync.GetCurrentIndex(), DeltaTime);

    }

    void ClVkRenderer::Destroy()
    {
        mBeginLayer.Destroy(mVkCtx);
        // mLayers3d.Destroy();
        mLayers2d.Destroy(mVkCtx);

        mEndLayer.Destroy(mVkCtx);

        cl_destroy_vk_context(&mVkCtx);
        cl_destroy_vk_loader(&mVkLoader);

        glslang_finalize_process();
    }

    void ClVkRenderer::Render()
    {
        VK_CHECK(vkWaitForFences(mVkCtx.Device, 1, &mVkCtx.FrameSync.GetCurrentInFlightFence(), true, UINT64_MAX));

        u32 img_index;
        VkResult result = vkAcquireNextImageKHR(mVkCtx.Device, mVkCtx.Swapchain.Handle, UINT64_MAX, mVkCtx.FrameSync.GetCurrentWaitSemaphore(), nullptr, &img_index);
        if(result != VK_SUCCESS)
        {
            if(result == VK_ERROR_OUT_OF_DATE_KHR) 
            { RecreateSwapchain(); return; }
            else if ( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            { CLOG_ERROR("Failed to acquire Image in VkSwapchian: %s", string_VkResult(result)); }
        }

        VK_CHECK(vkResetFences(mVkCtx.Device, 1, &mVkCtx.FrameSync.GetCurrentInFlightFence()));
        VkCommandBuffer& draw_buffer = mVkCtx.DrawCmds.Buffers[mVkCtx.FrameSync.GetCurrentIndex()];
        VK_CHECK(vkResetCommandBuffer(draw_buffer, 0));

        Update(0.f);
        DrawFrame(draw_buffer, img_index);

        VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSubmitInfo submit_info =
        {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &mVkCtx.FrameSync.GetCurrentWaitSemaphore(),
            .pWaitDstStageMask = wait_stages,
            .commandBufferCount = 1,
            .pCommandBuffers = &mVkCtx.DrawCmds.Buffers[mVkCtx.FrameSync.GetCurrentIndex()],
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &mVkCtx.FrameSync.GetCurrentRenderSemaphore()
        };
        VK_CHECK(vkQueueSubmit(mVkCtx.Queues.Graphics.Handle, 1, &submit_info, mVkCtx.FrameSync.GetCurrentInFlightFence()));

        VkPresentInfoKHR  present_info =
        {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &mVkCtx.FrameSync.GetCurrentRenderSemaphore(),
            .swapchainCount = 1,
            .pSwapchains = &mVkCtx.Swapchain.Handle,
            .pImageIndices = &img_index
        };
        result = vkQueuePresentKHR(mVkCtx.Queues.Graphics.Handle, &present_info);
        if(result != VK_SUCCESS || m_bIsResized)
        {
            if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_bIsResized)
            { RecreateSwapchain(); m_bIsResized = false; }
            else
            { CLOG_ERROR("Failed to present frame for render: %s", string_VkResult(result)); }
        }

        VK_CHECK(vkDeviceWaitIdle(mVkCtx.Device));
        mVkCtx.FrameSync.SetNextFrameIndex();
    }

    void ClVkRenderer::DrawFrame(const VkCommandBuffer& DrawBuffer, const u32 &ImageIndex)
    {
        VkCommandBufferBeginInfo begin_info =
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
            .pInheritanceInfo = nullptr
        };

        VK_CHECK(vkBeginCommandBuffer(DrawBuffer, &begin_info));

        mBeginLayer.DrawFrame(mVkCtx, DrawBuffer, ImageIndex);
        // mLayers3d.DrawFrame(DrawBuffer, ImageIndex);
        mLayers2d.DrawFrame(mVkCtx, DrawBuffer, ImageIndex);

        mEndLayer.DrawFrame(mVkCtx, DrawBuffer, ImageIndex);

        VK_CHECK(vkEndCommandBuffer(DrawBuffer));
    }

    void ClVkRenderer::CleanupSwapchain()
    {
        CLOG_INFO("Cleaning VkSwapchain and VkFramebuffers...");

        mBeginLayer.CleanupFramebuffers(mVkCtx);
        // mLayers3d.CleanupFramebuffers();
        mLayers2d.CleanupFramebuffers(mVkCtx);

        mEndLayer.CleanupFramebuffers(mVkCtx);
        cl_destroy_vk_swapchain(mVkCtx.Device, &mVkCtx.Swapchain);

        CLOG_INFO("VkSwapchain and VkFramebuffers cleaned.");
    }

    void ClVkRenderer::RecreateSwapchain()
    {
        CLOG_INFO("Recreating VkSwapchain and Framebuffers.");

        VK_CHECK(vkDeviceWaitIdle(mVkCtx.Device));
        CleanupSwapchain();

        ClPlatform* platform = ClPlatform::Get();
        i32 width, height;
        platform->GetDrawableSize(&width, &height);

        mVkCtx.Swapchain = cl_create_vk_swapchain(
            mVkCtx.Device, mVkCtx.PhysicalDevice, 
            mVkLoader.Surface, cl_get_vk_queues_indices_list(&mVkCtx.Queues), 
            width, height
        );
            
        mBeginLayer.RecreateFramebuffers(mVkCtx, nullptr);
        // mLayers3d.RecreateFramebuffers(&mDepthImage);
        mLayers2d.RecreateFramebuffers(mVkCtx, nullptr);

        mEndLayer.RecreateFramebuffers(mVkCtx, nullptr);

        CLOG_INFO("VkSwapchain and VkFramebuffers recreated.");
    }
}
