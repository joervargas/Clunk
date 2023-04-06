#include "ClVkColorImage.h"

#include "ClVkImage.h"
#include "ClVkImageView.h"
#include <ClVkContext/ClVkPhysicalDevice.h>
#include <ClVkContext/ClVkDevice.h>

namespace Clunk
{
    void ClVkColorImage::Create(const VkExtent2D &SwapchainExtent, const VkFormat& SwapchainFormat, const VkSampleCountFlagBits& MsaaSampleCount)
    {
        ClVkImage::Create(
            SwapchainExtent.width, SwapchainExtent.height,
            1, 1,
            SwapchainFormat,
            MsaaSampleCount,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            &m_image,
            &m_allocation
        );

        ClVkImageView::Create(
            &m_image,
            1,
            SwapchainFormat,
            VK_IMAGE_ASPECT_COLOR_BIT,
            &m_imageView
        );
    }

    void ClVkColorImage::Destroy()
    {
        ClVkDevice* Device = ClVkDevice::Get();
        Device->DestroyImageView(m_imageView, nullptr);

        ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
        Allocator->DestroyImage(m_image, m_allocation);
    }
}