#pragma once

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <ClVkContext/ClVkDevice.h>

#include <Defines/Defines.h>
#include <Core/Exception.h>
#include <Core/Logger.h>

namespace Clunk
{
    class ClVkImageView
    {
    public:

        static void Create(
                VkImage* Image,
                u32 MipLevels,
                VkFormat Format,
                VkImageAspectFlags AspectFlags,
                VkImageView* ImageView
            )
        {
            if(!Image) { THROW_EXCEPTION("VkImage* is null."); }
            if(!ImageView) { THROW_EXCEPTION("VkImageView* is null."); }

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = *Image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = Format;
            viewInfo.subresourceRange.aspectMask = AspectFlags;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = MipLevels;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            ClVkDevice* Device = ClVkDevice::Get();
            VK_CHECK(Device->CreateImageView(&viewInfo, nullptr, ImageView));
        }
    };
}