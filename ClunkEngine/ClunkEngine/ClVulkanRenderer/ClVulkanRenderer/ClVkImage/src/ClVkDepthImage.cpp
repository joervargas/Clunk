#include "ClVkDepthImage.h"
#include "ClVkImage.h"
#include "ClVkImageView.h"

#include <ClVkContext/ClVkDevice.h>
#include <ClVkContext/ClVkMemAllocator.h>

namespace Clunk
{
    void ClVkDepthImage::Create(const VkExtent2D& SwapchainExtent, const VkSampleCountFlagBits& MsaaSampleCount)
    {
        m_format = FindFormat();

        ClVkImage::Create(
            SwapchainExtent.width, SwapchainExtent.height,
            1, 1,
            m_format,
            MsaaSampleCount,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            &m_image,
            &m_allocation
        );

        ClVkImageView::Create(
            &m_image,
            1,
            m_format,
            VK_IMAGE_ASPECT_DEPTH_BIT,
            &m_imageView
        );
    }

    void ClVkDepthImage::Destroy()
    {
        ClVkDevice* Device = ClVkDevice::Get();
        Device->DestroyImageView(m_imageView, nullptr);

        ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
        Allocator->DestroyImage(m_image, m_allocation);
    }

    VkFormat ClVkDepthImage::FindFormat()
    {
        std::vector<VkFormat> candidates{
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT
        };
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkFormatFeatureFlags features = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    
        return ClVkImage::FindSupportedFormat(candidates, tiling, features);
    }
}