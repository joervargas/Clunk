#include "ClVkTexture.h"
#include "ClVkImage.h"
#include "ClVkImageView.h"
#include "ClVkSampler.h"

#include <STB_Image/stb_image.h>

#include <filesystem>
#include <cmath>
// #include <unistd.h>

#include <Core/Exception.h>
#include <ClVkBuffer/ClVkBuffer.h>
#include <ClVkContext/ClVkMemAllocator.h>
#include <ClVkCmds/ClVkCmdPools.h>
#include <ClVkCmds/ClVkCmds.h>
#include <ClVkContext/ClVkQueueFamilies.h>
#include <ClVkSwapchain/ClVkSwapchain.h>

namespace Clunk
{
    void ClVkTexture::FromFile(const char *FilePath)
    {
        if(std::filesystem::path(FilePath).extension() == ".ktx" ||
            std::filesystem::path(FilePath).extension() == ".ktx2")
        {
            // CreateKTX(FilePath);
        }
        else
        {
            CreateSTD(FilePath);
        }
    }

    void ClVkTexture::Destroy()
    {
        ClVkDevice* Device = ClVkDevice::Get();
        Device->DestroySampler(m_sampler, nullptr);
        Device->DestroyImageView(m_imageView, nullptr);

        ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
        Allocator->DestroyImage(m_image, m_allocation);
    }

    void ClVkTexture::CreateSTD(const char *FilePath, b8 bGenerateMipMaps)
    {
        // CLOG_WARN("The current directory is %s", get_current_dir_name());
        i32 width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* pixels = stbi_load(FilePath, &width, &height, &channels, STBI_rgb_alpha);

        m_baseWidth = static_cast<u32>(width);
        m_baseHeight = static_cast<u32>(height);

        m_mipLevels = 1;
        if(bGenerateMipMaps)
        {
            m_mipLevels = static_cast<u32>(std::floor(std::log2(std::max(width, height)))) + 1;
        }
        m_layers = 1;
        VkDeviceSize imageSize = width * height * 4;

        if(!pixels) { THROW_EXCEPTION("Failed to load texture image: %s", FilePath); }

        VkFormat texFormat = VK_FORMAT_R8G8B8A8_SRGB;
        // if(channels == 3) { texFormat = VK_FORMAT_R8G8B8_SRGB; }

        VkBuffer stagingBuffer;
        VmaAllocation stagingAllocation;

        ClVkBuffer::Create(
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            imageSize, &stagingBuffer,
            VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
            &stagingAllocation
        );

        ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
        // Copy data to memory
        void* data;
        Allocator->MapMemory(stagingAllocation, &data);
            memcpy(data, pixels, (size_t)imageSize);
        Allocator->UnmapMemory(stagingAllocation);

        // cleanup pixels array (no longer needed)
        stbi_image_free(pixels);

        // ClVkSwapchain* swapchain = ClVkSwapchain::Get();
        // VkSampleCountFlagBits msaaSamples = swapchain->GetMsaaSamples();

        VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        if(bGenerateMipMaps)
        {
            imageUsageFlags = imageUsageFlags | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        }
        ClVkImage::Create(
            width, height,
            m_mipLevels, m_layers, 
            texFormat, VK_SAMPLE_COUNT_1_BIT , VK_IMAGE_TILING_OPTIMAL,
            imageUsageFlags,
            &m_image,
            &m_allocation
        );

        // Begin Single time commands
        ClVkCmdPools* CmdPools = ClVkCmdPools::Get();
        VkCommandBuffer GfxBuffer = ClVkCmds::BeginSingleTimeCmd(*CmdPools->GetGraphicsPool());

        ClVkImage::TransitionLayout(GfxBuffer, 
            m_image, m_mipLevels, m_layers, 
            VK_IMAGE_LAYOUT_UNDEFINED,  // Src Layout
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL // Dst Layout
        );

        ClVkImage::CopyBuffer(GfxBuffer,
            stagingBuffer, m_image, 
            width, height, m_mipLevels
        );

        if(bGenerateMipMaps) // If generating MipMaps
        {
            ClVkImage::GenerateMipMaps(
                GfxBuffer, m_image, 
                texFormat, 
                m_baseWidth, m_baseHeight, 
                m_mipLevels
            );
        }
        else // Else transition image normally
        {
            ClVkImage::TransitionLayout(
                GfxBuffer, m_image, 
                m_mipLevels, m_layers, 
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, // Src Layout
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL  // Dst Layout
            );
        }

        // End and submit single time commands;
        ClVkQueueFamilies* Queues = ClVkQueueFamilies::Get();
        ClVkCmds::EndSingleTimeCmd(*CmdPools->GetGraphicsPool(), GfxBuffer, *Queues->Graphics());

        ClVkImageView::Create(
            &m_image,
            m_mipLevels, texFormat,
            VK_IMAGE_ASPECT_COLOR_BIT,
            &m_imageView
        );

        ClVkSampler::Create(m_mipLevels, VK_SAMPLE_COUNT_1_BIT, &m_sampler);

        Allocator->DestroyBuffer(stagingBuffer, stagingAllocation);
    }
}