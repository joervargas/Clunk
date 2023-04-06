#pragma once

#include "ClVkBuffer.h"
#include <ClVkContext/ClVkMemAllocator.h>
#include <ClVkCmds/ClVkCmds.h>

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <memory>
#include <Defines/Defines.h>
#include <PCH/pch.h>

namespace Clunk
{
    class ClVkArrayBuffer
    {
    public:

        VkBuffer buffer;
        VmaAllocation allocation;
        u32 len{0};

        template<class T>
        void Create(
                const VkCommandPool& CmdPool,
                const VkQueue& Queue,
                const std::vector<T>& Values,
                VkBufferUsageFlagBits UsageFlags
            )
        {
            len = static_cast<u32>(Values.size());
            VkDeviceSize bufferSize = sizeof(Values[0]) * len;

            VkBuffer stagingBuffer;
            VmaAllocation stagingAllocation;

            ClVkBuffer::Create(
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                bufferSize,
                &stagingBuffer,
                VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
                &stagingAllocation
            );

            ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
            // Copy data to memory
            void* pData;
            // Map *pData to allocated memory
            Allocator->MapMemory(stagingAllocation, &pData);
            // Copy values to the pData
                memcpy(pData, Values.data(), (size_t)bufferSize);
            // Unmap the memory
            Allocator->UnmapMemory(stagingAllocation);

            ClVkBuffer::Create(
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | UsageFlags,
                bufferSize,
                &buffer,
                0,
                &allocation
            );

            CopyBuffer(CmdPool, Queue, stagingBuffer, buffer, bufferSize);

            Allocator->DestroyBuffer(stagingBuffer, stagingAllocation);
        }


        void Destroy();


    private:

        void CopyBuffer(
            const VkCommandPool& CmdPool,
            const VkQueue& Queue,
            VkBuffer& SrcBuffer,
            VkBuffer& DstBuffer,
            VkDeviceSize BufferSize
        );
    
    };
}