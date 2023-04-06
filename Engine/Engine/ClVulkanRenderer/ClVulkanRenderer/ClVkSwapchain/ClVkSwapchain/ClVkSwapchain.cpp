#include "ClVkSwapchain.h"
#include <ClVkContext/ClVkPhysicalDevice.h>
// #include <ClVkContext/ClVkDevice.h>
#include <ClVkContext/ClVkSurface.h>
#include <ClVkContext/ClVkQueueFamilies.h>
#include <ClVkImage/ClVkImageView.h>

#include <Core/Logger.h>
#include <Core/Exception.h>

#include <SDL2/SDL_vulkan.h>
#include <ClPlatforms/ClPlatform.h>

namespace Clunk
{

    ClVkSwapchain ClVkSwapchain::m_ClVkSwapchain;

    void ClVkSwapchain::Create(u32 framesInFlight)
    {
        CLOG_INFO("Creating Vk Swapchain...")

        ClVkPhysicalDevice* PhysDevice = ClVkPhysicalDevice::Get();
        ClVkSurface* Surface = ClVkSurface::Get();
        Surface->QueryDetails(*PhysDevice->Handle());
        
        m_framesInFlight = framesInFlight;
        VkSurfaceFormatKHR selectedFormat = ChooseSwapSurfaceFormat(Surface->details.formats);
        VkPresentModeKHR selectedPresentMode = ChooseSwapPresentModes(Surface->details.presentModes);
        m_extent = ChooseSwapExtent(Surface->details.capabilities);

        m_imageCount = Surface->details.capabilities.minImageCount + 1;
        if(Surface->details.capabilities.maxImageCount > 0 &&
            m_imageCount > Surface->details.capabilities.maxImageCount)
        {
            m_imageCount = Surface->details.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = *Surface->Handle();
        createInfo.minImageCount = m_imageCount;
        createInfo.imageFormat = selectedFormat.format;
        createInfo.imageColorSpace = selectedFormat.colorSpace;
        createInfo.imageExtent = m_extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        ClVkQueueFamilies* QueueFamilies = ClVkQueueFamilies::Get();
        std::vector<u32> queueFamilyIndices = QueueFamilies->GetIndexList();

        if(QueueFamilies->Indices()->graphics != QueueFamilies->Indices()->present)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = (u32)queueFamilyIndices.size();
            createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = Surface->details.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = selectedPresentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        ClVkDevice* Device = ClVkDevice::Get();
        m_pDevice = Device->Handle();
        VK_CHECK(Device->CreateSwapchainKHR(&createInfo, nullptr, &m_swapchain));
        vkGetSwapchainImagesKHR(*Device->Handle(), m_swapchain, &m_imageCount, nullptr);
        images.resize(m_imageCount);
        VK_CHECK(vkGetSwapchainImagesKHR(*Device->Handle(), m_swapchain, &m_imageCount, images.data()));

        m_format = selectedFormat.format;
        CreateImageViews();

        CLOG_INFO("Vk Swapchain created.");

        // TODO: Move MSAA settings to a config file
        m_maxMsaaSamples = GetMaxUsableSampleCount();
        m_msaaSampleCount = m_maxMsaaSamples;
        colorImage.Create(m_extent, m_format, m_msaaSampleCount);

        depthImage.Create(m_extent, m_msaaSampleCount);

        if(!frameSync.IsInitialized())
        {
            frameSync.Create(framesInFlight);
        }
    }


    void ClVkSwapchain::Destroy()
    {   
        m_framebuffers.Destroy();
        frameSync.Destroy();

        depthImage.Destroy();
        colorImage.Destroy();

        DestroySwapchain();
    }

    void ClVkSwapchain::Reset()
    {
        // Handle window minimization
        ClPlatform* Platform = ClPlatform::Get();
        i32 width{0}, height{0};
        Platform->GetDrawableSize(&width, &height);
        while (width == 0 || height == 0)
        {
            Platform->GetDrawableSize(&width, &height);
            Platform->Wait();
        }

        // Wait for resources that may be in use
        ClVkDevice* Device = ClVkDevice::Get();
        Device->WaitIdle();

        // Destroy();
        // DestroyFramebuffers(false);
        m_framebuffers.Destroy();
        depthImage.Destroy();
        colorImage.Destroy();
        DestroySwapchain();

        Create(m_framesInFlight);
        // SetFramebuffers(RenderPass, 0);
    }

    void ClVkSwapchain::CreateFrameBuffers(const VkRenderPass& RenderPass)
    {
        m_framebuffers.Create(imageViews, *depthImage.GetView(), *colorImage.GetView(), m_extent, RenderPass);
    }

    VkFramebuffer ClVkSwapchain::GetFrameBuffer(u32 Index)
    {
        return m_framebuffers[Index];
    }

    void ClVkSwapchain::SetFrameSync(const u32 &FramesInFlightCount)
    {
        frameSync.Count = FramesInFlightCount;
        frameSync.ImageAvailableSemaphores.resize(frameSync.Count);
        frameSync.RenderFinishedSemaphores.resize(frameSync.Count);
        frameSync.InFlightFences.resize(frameSync.Count);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        
        ClVkDevice* Device = ClVkDevice::Get();
        for(size_t i = 0; i < frameSync.Count; i++)
        {
            VK_CHECK(Device->CreateSemaphore(&semaphoreInfo, nullptr, &frameSync.ImageAvailableSemaphores[i]));
            VK_CHECK(Device->CreateSemaphore(&semaphoreInfo, nullptr, &frameSync.RenderFinishedSemaphores[i]));
            VK_CHECK(Device->CreateFence(&fenceInfo, nullptr, &frameSync.InFlightFences[i]));
        }
    }

    VkResult ClVkSwapchain::AquireNextImage(const u64& Timeout, const u32& FrameIndex, u32 *pImageIndex)
    {
        // ClVkDevice* Device = ClVkDevice::Get();
        return vkAcquireNextImageKHR(*m_pDevice ,m_swapchain, Timeout, frameSync.ImageAvailableSemaphores[FrameIndex], VK_NULL_HANDLE, pImageIndex);
    }

    VkResult ClVkSwapchain::WaitForInFlightFence(const u64 &Timeout, const u32& FrameIndex)
    {
        // ClVkDevice* Device = ClVkDevice::Get();
        return vkWaitForFences(*m_pDevice, 1, &frameSync.InFlightFences[FrameIndex], true, Timeout);
    }

    VkResult ClVkSwapchain::ResetInFlightFence(const u32 &FrameIndex)
    {
        // ClVkDevice* Device = ClVkDevice::Get();
        return vkResetFences(*m_pDevice, 1, &frameSync.InFlightFences[FrameIndex]);
    }

    VkResult ClVkSwapchain::SubmitFrame(const VkCommandBuffer &DrawBuffer, const u32 &FrameIndex)
    {
        VkSemaphore waitSemaphores[] = { frameSync.ImageAvailableSemaphores[FrameIndex] };
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        VkSemaphore signalSemaphores[] = { frameSync.RenderFinishedSemaphores[FrameIndex] };

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &DrawBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        ClVkQueueFamilies* QueueFamilies = ClVkQueueFamilies::Get();
        return vkQueueSubmit(*QueueFamilies->Graphics(), 1, &submitInfo, frameSync.InFlightFences[FrameIndex]);
    }

    VkResult ClVkSwapchain::PresentFrame(const u32 &FrameIndex, const u32 &ImageIndex)
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &frameSync.RenderFinishedSemaphores[FrameIndex];
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_swapchain;
        presentInfo.pImageIndices = &ImageIndex;
        presentInfo.pResults = nullptr;

        ClVkQueueFamilies* QueueFamilies = ClVkQueueFamilies::Get();
        return vkQueuePresentKHR(*QueueFamilies->Present(), &presentInfo);
    }

    VkSurfaceFormatKHR ClVkSwapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &AvailableFormats)
    {
        for(const auto& availableFormat : AvailableFormats)
        {
            if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }
        return AvailableFormats[0];
    }


    VkPresentModeKHR ClVkSwapchain::ChooseSwapPresentModes(const std::vector<VkPresentModeKHR> &AvailablePresentModes)
    {
        for(const auto& availablePresenMode : AvailablePresentModes)
        {
            if(availablePresenMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresenMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }


    VkExtent2D ClVkSwapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &Capabilities)
    {
        if(Capabilities.currentExtent.width != UINT32_MAX)
        {
            return Capabilities.currentExtent;
        }
        else
        {
            ClPlatform* Platform = ClPlatform::Get();
            i32 width, height;
            // SDL_Vulkan_GetDrawableSize(Window, &width, &height);
            Platform->GetDrawableSize(&width, &height);
            VkExtent2D actualExtent = {
                static_cast<u32>(width),
                static_cast<u32>(height)
            };

            actualExtent.width = std::clamp(
                actualExtent.width, 
                Capabilities.minImageExtent.width, 
                Capabilities.maxImageExtent.width
            );
            actualExtent.height = std::clamp(
                actualExtent.height,
                Capabilities.minImageExtent.height,
                Capabilities.maxImageExtent.height
            );
            
            return actualExtent;
        }
    }

    VkSampleCountFlagBits ClVkSwapchain::GetMaxUsableSampleCount()
    {
        ClVkPhysicalDevice* physDevice = ClVkPhysicalDevice::Get();
        VkSampleCountFlags counts = physDevice->Properties()->properties.limits.framebufferColorSampleCounts & 
            physDevice->Properties()->properties.limits.framebufferDepthSampleCounts;
        
        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }
        return VK_SAMPLE_COUNT_1_BIT;
    }

    void ClVkSwapchain::CreateImageViews()
    {
        imageViews.resize(images.size());

        for(size_t i = 0; i < images.size(); i++)
        {
            ClVkImageView::Create(
                &images[i],
                1,
                m_format,
                VK_IMAGE_ASPECT_COLOR_BIT,
                &imageViews[i]
            );
        }
    }

    void ClVkSwapchain::DestroySwapchain()
    {
        CLOG_INFO("Destroying Vk Swapchain...");

        for(VkImageView imageView: imageViews)
        {
            vkDestroyImageView(*m_pDevice, imageView, nullptr);
        }
        vkDestroySwapchainKHR(*m_pDevice, m_swapchain, nullptr);

        CLOG_INFO("Vk Swapchain destroyed.");
    }

}
