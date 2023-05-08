#pragma once

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <ClVkContext/ClVkMemAllocator.h>
#include <ClVkContext/ClVkPhysicalDevice.h>

#include <Core/Exception.h>
#include <Core/Logger.h>
#include <STB_Image/stb_image.h>


namespace Clunk
{
    class ClVkImage
    {
    public:

        static void Create(
                u32 Width, 
                u32 Height,
                u32 MipLevels, 
                u32 ArrayLayers,
                VkFormat Format,
                VkSampleCountFlagBits SampleCountFlags,
                VkImageTiling Tiling,
                VkImageUsageFlags UsageFlags,
                VkImage* Image,
                VmaAllocation* Allocation
            )
        {
            if(!Image) { THROW_EXCEPTION("VkImage* Image is null."); }
            if(!Allocation) { THROW_EXCEPTION("VmaAllocation is null"); }

            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = Width;
            imageInfo.extent.height = Height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = MipLevels;
            imageInfo.arrayLayers = ArrayLayers;
            imageInfo.format = Format;
            imageInfo.tiling = Tiling;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = UsageFlags;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageInfo.samples = SampleCountFlags;

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            
            ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
            VK_CHECK(Allocator->CreateImage(&imageInfo, &allocInfo, Image, Allocation, nullptr));
        }


        static void CopyBuffer(
                const VkCommandBuffer CmdBuffer,
                const VkBuffer& Buffer,
                const VkImage& Image,
                u32 Width, u32 Height,
                u32 MipLevels
            )
        {
            VkBufferImageCopy copyRegion{};
            copyRegion.bufferOffset = 0;
            copyRegion.bufferRowLength = 0;
            copyRegion.bufferImageHeight = 0;

            copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.imageSubresource.mipLevel = 0;
            copyRegion.imageSubresource.baseArrayLayer = 0;
            copyRegion.imageSubresource.layerCount = 1;

            copyRegion.imageOffset = { 0, 0, 0 };
            copyRegion.imageExtent = { Width, Height, 1 };

            vkCmdCopyBufferToImage(
                CmdBuffer, Buffer, Image, 
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                1, &copyRegion
            );
        }
    
        static void TransitionLayout(
                const VkCommandBuffer& CmdBuffer,
                VkImage& Image,
                u32 MipLevels, u32 LayerCount,
                VkImageLayout OldLayout,
                VkImageLayout NewLayout
            )
        {
            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = OldLayout;
            barrier.newLayout = NewLayout;

            // Used to transfer QueueFamily ownership
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

            barrier.image = Image;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = MipLevels;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = LayerCount;

            VkPipelineStageFlags srcStage, dstStage;
            VkAccessFlags srcAccess, dstAccess;
            SetAccessMasksAndStages(
                OldLayout, NewLayout,
                srcAccess, dstAccess,
                srcStage, dstStage
            );

            barrier.srcAccessMask = srcAccess;
            barrier.dstAccessMask = dstAccess;

            vkCmdPipelineBarrier(
                CmdBuffer,
                srcStage, dstStage, // Pipeline Stage transition
                0, // Dependency flags
                0, nullptr, // Memory barrier count and handles
                0, nullptr, // Buffer Memory barrier count and handles
                1, &barrier // Image Memory barrier count and handles
            );
        }

        static void GenerateMipMaps(const VkCommandBuffer& CmdBuffer, const VkImage& Image, VkFormat ImageFormat, u32 Width, u32 Height, u32 MipLevels)
        {
            VkFormatProperties formatProps{};
            ClVkPhysicalDevice* physDevice = ClVkPhysicalDevice::Get();
            physDevice->GetPhysDeviceFormatProperties(ImageFormat, &formatProps);
            if(!(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
            {
                THROW_EXCEPTION("Image format not supported on this device for linear blitting! Failed to Generate MipMaps!");
                return;
            }

            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.image = Image;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;
            barrier.subresourceRange.levelCount = 1;

            i32 mipWidth = Width;
            i32 mipHeight = Height;
            for(u32 i = 1; i < MipLevels; i++)
            {
                barrier.subresourceRange.baseMipLevel = i -1;
                barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

                vkCmdPipelineBarrier(
                    CmdBuffer,
                    VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                    0, nullptr, // memory barrier count and pointer
                    0, nullptr, // buffer memory barrier count and pointer
                    1, &barrier // image memory barrier count and pointer
                );

                VkImageBlit blit{};
                blit.srcOffsets[0] = {0, 0, 0};
                blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
                blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.srcSubresource.mipLevel = i - 1;
                blit.srcSubresource.baseArrayLayer = 0;
                blit.srcSubresource.layerCount = 1;
                blit.dstOffsets[0] = { 0, 0, 0 };
                blit.dstOffsets[1] = { mipWidth > 1? mipWidth / 2 : 1, mipHeight > 1? mipHeight / 2 : 1, 1};
                blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.dstSubresource.mipLevel = i;
                blit.dstSubresource.baseArrayLayer = 0;
                blit.dstSubresource.layerCount = 1;

                vkCmdBlitImage(
                    CmdBuffer, 
                    Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    1, &blit,
                    VK_FILTER_LINEAR
                );

                barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                vkCmdPipelineBarrier(
                    CmdBuffer,
                    VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                    0, nullptr, // memory barrier count and pointer
                    0, nullptr, // buffer memory barrier count and pointer
                    1, &barrier // image memory barrier count and pointer
                );

                if(mipWidth > 1) { mipWidth /=2; }
                if(mipHeight > 1) { mipHeight /=2; }
            }

            barrier.subresourceRange.baseMipLevel = MipLevels - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(
                CmdBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                0, nullptr, // memory barrier count and pointer
                0, nullptr, // buffer memory barrier count and pointer
                1, &barrier // image memory barrier count and pointer
            );
        }

        static VkFormat FindSupportedFormat(
                const std::vector<VkFormat>& Candidates,
                VkImageTiling Tiling,
                VkFormatFeatureFlags Features
            )
        {
            ClVkPhysicalDevice* PhysDevice = ClVkPhysicalDevice::Get();
            if(!PhysDevice) { THROW_EXCEPTION("VkPhysicalDevice is null! Can't find format."); }

            for(VkFormat format : Candidates)
            {
                VkFormatProperties props;
                PhysDevice->GetPhysDeviceFormatProperties(format, &props);

                if(Tiling == VK_IMAGE_TILING_LINEAR && 
                    (props.linearTilingFeatures & Features) == Features)
                {
                    return format;
                }
                else if(Tiling == VK_IMAGE_TILING_OPTIMAL &&
                    (props.optimalTilingFeatures & Features) == Features)
                {
                    return format;
                }
            }
            THROW_EXCEPTION("Failed to find a supported Image Format!");
        }

    private:

        static void SetAccessMasksAndStages(
                const VkImageLayout& OldLayout,
                const VkImageLayout& NewLayout,
                VkAccessFlags& SrcAccessMask,
                VkAccessFlags& DstAccessMask,
                VkPipelineStageFlags& SrcStage,
                VkPipelineStageFlags& DstStage
            )
        {
            if(OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && 
                NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
            {
                SrcAccessMask = 0;
                DstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

                SrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                DstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            }
            else if(OldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            {
                SrcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                DstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                SrcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                DstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            }
            else
            {
                THROW_EXCEPTION("Unsuported Image Layout Transition!");
            }
        }

    };
}