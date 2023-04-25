#pragma once

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <ClVkContext/ClVkMemAllocator.h>

#include <Defines/Defines.h>
#include <Core/Exception.h>
#include <Core/Logger.h>


namespace Clunk
{
    class ClVkBuffer
    {
    public:

        static void Create(
                VkBufferUsageFlags UsageFlags,
                VkDeviceSize Size,
                VkBuffer* Buffer,
                VmaAllocationCreateFlags AllocFlags,
                VmaAllocation* Allocation
                // b8 bTransfer = false,
                // u32 QueueCount = 0,
                // u32* pQueueIndices = nullptr
            )    
        {

            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = Size;
            bufferInfo.usage = UsageFlags;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            // if(bTransfer && pQueueIndices && QueueCount > 0)
            // {
            //     bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
            //     bufferInfo.queueFamilyIndexCount = QueueCount;
            //     bufferInfo.pQueueFamilyIndices = pQueueIndices;
            // }
            // else
            // {
            //     bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            // }

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = AllocFlags;

            ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
            VK_CHECK(Allocator->CreateBuffer(&bufferInfo, &allocInfo, Buffer, Allocation, nullptr))
        }

    private:



    };
}