#pragma once

#include "ClVkFramebuffers.h"
#include "FrameSync.h"

#include <ClVkPipeline/ClVkRenderPass.h>
#include <ClVkContext/ClVkDevice.h>
#include <ClVkImage/ClVkDepthImage.h>
#include <ClVkImage/ClVkColorImage.h>

#include <vulkan/vulkan.h>

#include <Defines/Defines.h>
#include <PCH/pch.h>


namespace Clunk
{
    class ClVkSwapchain
    {
    public:

        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        ClVkColorImage colorImage;
        ClVkDepthImage depthImage;
        ClVkFrameSync frameSync;

        void Create(u32 FramesInFlight);

        void Destroy();

        void Reset();

        void CreateFrameBuffers(const VkRenderPass& RenderPass);
        VkFramebuffer GetFrameBuffer(u32 Index);

        static ClVkSwapchain* Get() { return &m_ClVkSwapchain; }

        const VkSwapchainKHR* Handle() { return &m_swapchain; }

        const VkExtent2D* GetExtent() const { return &m_extent; }

        const VkFormat* GetFormat() { return &m_format; }
        
        const VkSampleCountFlagBits GetMsaaSamples() const { return m_msaaSampleCount; }

        const u32 GetImageCount() { return m_imageCount; }

        void SetFrameSync(const u32& FramesInFlightCount);

        VkResult AquireNextImage(const u64& Timeout, const u32& FrameIndex, u32* pImageIndex);
        VkResult WaitForInFlightFence(const u64& Timeout, const u32& FrameIndex);
        VkResult ResetInFlightFence(const u32& FrameIndex);
        VkResult SubmitFrame(const VkCommandBuffer& DrawBuffer, const u32& CurrentFrame);
        VkResult PresentFrame(const u32& CurrentFrame, const u32& ImageIndex);


    private:

        ClVkSwapchain() {}

        static ClVkSwapchain m_ClVkSwapchain;

        VkSwapchainKHR m_swapchain;
        VkExtent2D m_extent;
        VkFormat m_format;
        u32 m_imageCount;
        u32 m_framesInFlight;
        ClVkFramebuffers m_framebuffers;
        VkSampleCountFlagBits m_maxMsaaSamples;
        VkSampleCountFlagBits m_msaaSampleCount;
        VkDevice* m_pDevice;

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats);
        VkPresentModeKHR ChooseSwapPresentModes(const std::vector<VkPresentModeKHR>& AvailablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& VkSurfaceCapabilitiesKHR);

        VkSampleCountFlagBits GetMaxUsableSampleCount();

        void CreateImageViews();

        void DestroySwapchain();

    };
}
