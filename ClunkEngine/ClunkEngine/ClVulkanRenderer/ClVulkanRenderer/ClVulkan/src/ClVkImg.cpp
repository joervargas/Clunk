#include "ClVkImg.h"
#include "ClVkContext.h"
#include "ClVkBuffer.h"

// #include <ClRenderManager/ClBitmap.h>
#include <ClRenderManager/ClCubemapUtils.h>

#include <STB_Image/stb_image.h>


namespace Clunk::Vk
{

    void create_vk_image(
            VkDevice Device, VmaAllocator Allocator, 
            u32 Width, u32 Height, 
            VkFormat Format, VkImageTiling Tiling, 
            VkImageUsageFlags Usage, VkMemoryPropertyFlags Properties, 
            VkImage* pImage, VmaAllocation* pAllocation, 
            u32 MipLevels, VkImageCreateFlags Flags
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

        VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_NONE;
        VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_NONE;

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

    u32 bytes_per_vk_tex_format(VkFormat fmt)
    {
        switch (fmt)
        {
        case VK_FORMAT_R8_SINT:
        case VK_FORMAT_R8_UNORM:
            return 1;
        case VK_FORMAT_R16_SFLOAT:
            return 2;
        case VK_FORMAT_R16G16_SFLOAT:
        case VK_FORMAT_R16G16_SNORM:
            return 4;
        case VK_FORMAT_B8G8R8A8_UNORM:
        case VK_FORMAT_R8G8B8A8_UNORM:
            return 4;
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return 4 * sizeof(uint16_t);
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return 4 * sizeof(float);
        default:
            break;
        }
        return 0;
    }

    b8 cl_create_vk_image_from_data(ClVkContext &VkCtx, u32 Width, u32 Height, VkFormat Format, void *ImageData, ClVkImage *OutputImage, u32 LayerCount, VkImageCreateFlags Flags)
    {
        create_vk_image(
            VkCtx.Device, VkCtx.MemAllocator, 
            Width, Height, 
            Format, VK_IMAGE_TILING_OPTIMAL, 
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            &OutputImage->Handle, &OutputImage->Allocation,
            1, 
            Flags
        );

        return cl_update_vk_image(VkCtx, Width, Height, LayerCount, Format, ImageData, OutputImage);
    }

    b8 cl_update_vk_image(ClVkContext &VkCtx, u32 Width, u32 Height, u32 LayerCount, VkFormat Format, void *ImageData, ClVkImage *OutputImage)
    {
        u32 bytes_per_pixel = bytes_per_vk_tex_format(Format);

        VkDeviceSize layer_size = Width * Height * bytes_per_pixel;
        VkDeviceSize image_size = layer_size * LayerCount;

        VkBuffer staging_buffer;
        VmaAllocation staging_allocation;
        create_vk_buffer(
            VkCtx.Device, VkCtx.MemAllocator,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
            image_size, 
            &staging_buffer, &staging_allocation
        );

        map_vk_allocation_data(VkCtx.MemAllocator, staging_allocation, ImageData, image_size);

        VkCommandBuffer cmd = cl_begin_single_time_vk_command_buffer(VkCtx);

            transition_vk_image_layout(
                cmd, OutputImage->Handle, 
                Format, 
                VK_IMAGE_LAYOUT_UNDEFINED, 
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                LayerCount);

            copy_vk_cmd_buffer_to_image(cmd, staging_buffer, OutputImage->Handle, Width, Height, LayerCount);

            transition_vk_image_layout(
                cmd, OutputImage->Handle, 
                Format, 
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                LayerCount);
            
        cl_end_single_time_vk_command_buffer(VkCtx, cmd);

        vmaDestroyBuffer(VkCtx.MemAllocator, staging_buffer, staging_allocation);
        
          return true;
    }

    static void float24to32(int w, int h, const float* img24, float* img32)
    {
        const int num_pixels = w * h;
        for(int i = 0; i != num_pixels; i++)
        {
            *img32++ = *img24++;
            *img32++ = *img24++;
            *img32++ = *img24++;
            *img32++ = 1.0f;
        }
    }

    // static void uint24to32(int w, int h, const u8* img24, u8* img32)
    // {
    //     const int num_pixels = w * h;
    //     for(int i = 0; i != num_pixels; i++)
    //     {
    //         *img32++ = *img24++;
    //         *img32++ = *img24++;
    //         *img32++ = *img24++;
    //         *img32++ = 255;
    //     }
    // }

    ClVkImage cl_create_vk_cubemap_image(ClVkContext &VkCtx, std::vector<const char*> FileNames)
    {
        b8 correct_filecount = false;
        if (FileNames.size() == 1) { correct_filecount = true; }
        if (FileNames.size() == 6) { correct_filecount = true; }
        if(!correct_filecount) 
        {
            CLOG_ERROR("Cubemap FileCount must be either 1 or 6");
            return ClVkImage();
        }

        int width = 0, height = 0, channels = 0;

        ClBitmap cube;
        if(FileNames.size() == 1)
        {
            const float* img = stbi_loadf(FileNames[0], &width, &height, &channels, 3);
            // TODO: error check on img nullptr
            std::vector<float> pixels(width * height * 4);
            float24to32(width, height, img, pixels.data());
            // if(channels == 3) { channels = 4; }
            stbi_image_free((void*)img);

            ClBitmap in(width, height, 4, EBitmapFormat_Float, pixels.data());
            ClBitmap out = convertEquirectangularMapToVerticalCross(in);

            cube = convertVerticalCrossToCubeMapFaces(out);
            // stbi_image_free((void*)pixels);
        }
        if (FileNames.size() == 6)
        {
            cube = convertMultiFileToCubeMapFaces(FileNames, &width, &height);
        }

        VkFormat img_format = cube.GetFormat() == EBitmapFormat_Float ? VK_FORMAT_R32G32B32A32_SFLOAT : VK_FORMAT_R8G8B8A8_SRGB;
        u32 tex_width = static_cast<u32>(cube.GetWidth());
        u32 tex_height = static_cast<u32>(cube.GetHeight());
        VkDeviceSize img_size = ((tex_width * tex_height) * bytes_per_vk_tex_format(img_format)) * 6;

        VkBuffer staging_buffer;
        VmaAllocation staging_allocation;
        
        create_vk_buffer(VkCtx.Device, VkCtx.MemAllocator, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
            img_size, &staging_buffer, &staging_allocation);

        void* p_data = nullptr;
        vmaMapMemory(VkCtx.MemAllocator, staging_allocation, &p_data);
            memcpy(p_data, cube.Data.data(), static_cast<size_t>(img_size));
        vmaUnmapMemory(VkCtx.MemAllocator, staging_allocation);

        ClVkImage result;
        create_vk_image(
            VkCtx.Device, VkCtx.MemAllocator, 
            tex_width, tex_height, img_format, VK_IMAGE_TILING_OPTIMAL, 
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            &result.Handle, &result.Allocation, 1, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT
        );

        VkCommandBuffer cmd = cl_begin_single_time_vk_command_buffer(VkCtx);

            transition_vk_image_layout(
                cmd, result.Handle, 
                img_format, 
                VK_IMAGE_LAYOUT_UNDEFINED, 
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                6);

            copy_vk_cmd_buffer_to_image(cmd, staging_buffer, result.Handle, tex_width, tex_height, 6);

            transition_vk_image_layout(
                cmd, result.Handle, 
                img_format, 
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                6);
            
        cl_end_single_time_vk_command_buffer(VkCtx, cmd);

        vkDestroyBuffer(VkCtx.Device, staging_buffer, nullptr);
        vmaFreeMemory(VkCtx.MemAllocator, staging_allocation);

        // cl_create_vk_image_from_data(VkCtx, cube.GetWidth(), cube.GetHeight(), img_format, cube.Data.data(), &result, 6, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT);
        
        create_vk_image_view(
            VkCtx.Device, result.Handle, 
            img_format, VK_IMAGE_ASPECT_COLOR_BIT, 
            &result.View, 
            VK_IMAGE_VIEW_TYPE_CUBE, 6);
            
        return result;
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
