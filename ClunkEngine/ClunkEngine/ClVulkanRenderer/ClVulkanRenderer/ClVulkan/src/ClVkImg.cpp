#include "ClVkImg.h"
#include "ClVkContext.h"
#include "ClVkBuffer.h"

#include <STB_Image/stb_image.h>


namespace Clunk::Vk
{

    void create_vk_image(
            VkDevice Device, VmaAllocator Allocator, 
            u32 Width, u32 Height, 
            VkFormat Format, VkImageTiling Tiling, 
            VkImageUsageFlags Usage, VkMemoryPropertyFlags Properties, 
            VkImage* pImage, VmaAllocation* pAllocation, 
            VkImageCreateFlags Flags, u32 MipLevels
        )
    {
        const VkImageCreateInfo imageInfo = 
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = Flags,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = Format,
            .extent = VkExtent3D{ .width = Width, .height = Height, .depth = 1 },
            .mipLevels = MipLevels,
            .arrayLayers = (u32)((Flags == VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) ? 6 : 1),
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .tiling = Tiling,
            .usage = Usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
        };

        VmaAllocationCreateInfo allocInfo =
        {
            .usage = VMA_MEMORY_USAGE_AUTO
        };
        VK_CHECK(vmaCreateImage(Allocator, &imageInfo, &allocInfo, pImage, pAllocation, nullptr));
    }

    void create_vk_image_view(
            VkDevice Device, VkImage Image, 
            VkFormat Format, VkImageAspectFlags AspectFlags, 
            VkImageView *pImageView, 
            VkImageViewType ImageViewType, u32 LayerCount, u32 MipLevels
        )
    {
        const VkImageViewCreateInfo view_info =
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = Image,
            .viewType = ImageViewType,
            .format = Format,
            .subresourceRange =
            {
                .aspectMask = AspectFlags,
                .baseMipLevel = 0,
                .levelCount = MipLevels,
                .baseArrayLayer = 0,
                .layerCount = LayerCount
            }
        };

        VK_CHECK(vkCreateImageView(Device, &view_info, nullptr, pImageView));
    }

    void create_vk_sampler(VkDevice Device, VkSampler *pSampler)
    {
        const VkSamplerCreateInfo samplerInfo =
        {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .magFilter = VK_FILTER_LINEAR,
            .minFilter = VK_FILTER_LINEAR,
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .mipLodBias = 0.0f,
            .maxAnisotropy = 1,
            .compareEnable = VK_FALSE,
            .compareOp = VK_COMPARE_OP_ALWAYS,
            .minLod = 0.0f,
            .maxLod = 0.0f,
            .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
            .unnormalizedCoordinates = VK_FALSE
        };
        VK_CHECK(vkCreateSampler(Device, &samplerInfo, nullptr, pSampler));
    }

    VkFormat find_supported_vk_format(VkPhysicalDevice PhysicalDevice, const std::vector<VkFormat> &Candidates, VkImageTiling Tiling, VkFormatFeatureFlags Features)
    {
        for(VkFormat format : Candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(PhysicalDevice, format, &props);

            if(Tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & Features) == Features)
            {
                return format;
            }
            else if (Tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & Features) == Features)
            {
                return format;
            }
        }

        THROW_EXCEPTION("Failed to find supported format!\n");
        exit(EXIT_FAILURE);
    }

    void transition_vk_image_layout(VkCommandBuffer CmdBuffer, VkImage Image, VkFormat Format, VkImageLayout OldLayout, VkImageLayout NewLayout, u32 LayerCount, u32 MipLevels)
    {
        VkImageMemoryBarrier barrier =
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = 0,
            .dstAccessMask = 0,
            .oldLayout = OldLayout,
            .newLayout = NewLayout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = Image,
            .subresourceRange = VkImageSubresourceRange {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = MipLevels,
                .baseArrayLayer = 0,
                .layerCount = LayerCount
            },
        };

        VkPipelineStageFlags srcStage, dstStage;

        if( NewLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL ||
                (Format == VK_FORMAT_D16_UNORM) ||
                (Format == VK_FORMAT_X8_D24_UNORM_PACK32) ||
                (Format == VK_FORMAT_D32_SFLOAT) ||
                (Format == VK_FORMAT_S8_UINT) ||
                (Format == VK_FORMAT_D16_UNORM_S8_UINT) ||
                (Format == VK_FORMAT_D24_UNORM_S8_UINT)
            )
        {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            if(has_vk_stencil_component(Format))
            {
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        } 
        else 
        {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
        }

        if( OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if( OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && NewLayout == VK_IMAGE_LAYOUT_GENERAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if( OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        /* Convert back from read-only to updateable */
        else if( OldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		    srcStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		    dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        /* Convert from updateable texture to shader read-only */
        else if( OldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        /* Convert depth texture from undefined state to depth-stencil buffer */
        else if( OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        /* Wait for render pass to complete */
        else if( OldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = 0;
            
            srcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        /* Convert back from read-only to color attachment */
        else if( OldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && NewLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            srcStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }
        /* Convert from updateable texture to shader read-only */
        else if( OldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            srcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        /* Convert back from read-only to depth attachment */
        else if( OldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            srcStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            dstStage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        }
        /* Convert from updateable depth texture to shader read-only */
        else if( OldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL && NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            srcStage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }

        vkCmdPipelineBarrier(
            CmdBuffer,
            srcStage, dstStage,
            0,
            0, nullptr, // VkMemoryBarrier
            0, nullptr, // VkBufferMemoryBarrier
            1, &barrier // VkImageMemoryBarrier
        );
    }

    void copy_vk_cmd_buffer_to_image(VkCommandBuffer CmdBuffer, VkBuffer Buffer, VkImage Image, u32 Width, u32 Height, u32 LayerCount)
    {
        const VkBufferImageCopy copy_region =
        {
            .bufferOffset = 0,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource =
            {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = 0,
                .baseArrayLayer = 0,
                .layerCount = LayerCount
            },
            .imageOffset = VkOffset3D{ .x = 0, .y = 0, .z = 0 },
            .imageExtent = VkExtent3D{ .width = Width, .height = Height, .depth = 1 }
        };

        vkCmdCopyBufferToImage(CmdBuffer, Buffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);
    }

    VkFormat find_vk_format_depth_img(VkPhysicalDevice PhysicalDevice)
    {
        return find_supported_vk_format(
            PhysicalDevice, 
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT 
        );
    }

    bool has_vk_stencil_component(VkFormat Format)
    {
        return Format == VK_FORMAT_D32_SFLOAT_S8_UINT || Format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    ClVkImage cl_create_vk_image(ClVkContext &VkCtx, const char *Filename)
    {
        int width, height, channels;
        stbi_uc* pixels = stbi_load(Filename, &width, &height, &channels, STBI_rgb_alpha);
        if(!pixels)
        {
            THROW_EXCEPTION("Failed to load [%s] texture\n", Filename);
            fflush(stdout);
        }

        VkDeviceSize image_size = width * height * 4;
        VkBuffer staging_buffer;
        // VkDeviceMemory staging_memory;
        VmaAllocation staging_allocation;
        
        create_vk_buffer(VkCtx.Device, VkCtx.MemAllocator, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
            image_size, &staging_buffer, &staging_allocation);

        void* p_data;
        vmaMapMemory(VkCtx.MemAllocator, staging_allocation, &p_data);
            memcpy(p_data, pixels, static_cast<size_t>(image_size));
        vmaUnmapMemory(VkCtx.MemAllocator, staging_allocation);

        VkFormat img_format = VK_FORMAT_R8G8B8A8_SRGB;
        ClVkImage img;
        create_vk_image(
            VkCtx.Device, VkCtx.MemAllocator, 
            width, height, 
            img_format,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            &img.Handle, &img.Allocation);

        VkCommandBuffer cmd = cl_begin_single_time_vk_command_buffer(VkCtx);
            
            transition_vk_image_layout(
                cmd, img.Handle, 
                img_format, 
                VK_IMAGE_LAYOUT_UNDEFINED, 
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            copy_vk_cmd_buffer_to_image(cmd, staging_buffer, img.Handle, width, height);

            transition_vk_image_layout(
                cmd, img.Handle, 
                img_format, 
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            
        cl_end_single_time_vk_command_buffer(VkCtx, cmd);

        vkDestroyBuffer(VkCtx.Device, staging_buffer, nullptr);
        vmaFreeMemory(VkCtx.MemAllocator, staging_allocation);
        stbi_image_free(pixels);

        create_vk_image_view(VkCtx.Device, img.Handle, img_format, VK_IMAGE_ASPECT_COLOR_BIT, &img.View);

        return img;
    }

    ClVkImage cl_create_vk_depth_image(ClVkContext &VkCtx, u32 Width, u32 Height)
    {
        ClVkImage depthImg;
        VkFormat depthFormat = find_vk_format_depth_img(VkCtx.PhysicalDevice);

        create_vk_image(
            VkCtx.Device, VkCtx.MemAllocator,
            Width, Height, depthFormat,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            &depthImg.Handle, &depthImg.Allocation
        );

        create_vk_image_view(VkCtx.Device, depthImg.Handle, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, &depthImg.View);

        VkCommandBuffer cmdBuffer = cl_begin_single_time_vk_command_buffer(VkCtx);
            transition_vk_image_layout(
                cmdBuffer, 
                depthImg.Handle, depthFormat, 
                VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
            );
        cl_end_single_time_vk_command_buffer(VkCtx, cmdBuffer);

        return depthImg;
    }

    void cl_destroy_vk_image(ClVkContext& VkCtx, ClVkImage* pImage)
    {
        vkDestroyImage(VkCtx.Device, pImage->Handle, nullptr);
        vkDestroyImageView(VkCtx.Device, pImage->View, nullptr);
        vmaFreeMemory(VkCtx.MemAllocator, pImage->Allocation);

        pImage->Handle = nullptr;
        pImage->View = nullptr;
        pImage->Allocation = nullptr;
    }
}