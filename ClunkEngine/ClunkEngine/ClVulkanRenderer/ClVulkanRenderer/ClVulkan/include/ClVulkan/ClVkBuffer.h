#pragma once

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>
#include <PCH/pch.h>

class ClVkContext;

namespace Clunk::Vk
{
    /**
     * @brief Finds the memory type index given the memory type bits and memory properties.
     * 
     * @param PhysDevice VkPhysicalDevice handle
     * @param TypeFilter VkMemoryRequirements::memoryTypeBits to filter by
     * @param Properties VkMemoryPropertyFlags
     * @return u32 
     */
    u32 find_vk_memory_type_index(const VkPhysicalDevice PhysDevice, u32 TypeFilter, VkMemoryPropertyFlags Properties);

    /**
     * @brief Populates VkBuffer and VmaAllocation handles
     * 
     * @param Device VkDevice handle
     * @param VmaAllocator VmaAllocator handle
     * @param BufferUsage VkBufferUsageFlags
     * @param AllocationCreateFlags VmaAllocationCreateFlags
     * @param Size VkDeviceSize ( buffer size )
     * @param pBuffer VkBuffer* handle to populate
     * @param pAllocation VmaAllocation* handle to populate
     */
    void create_vk_buffer(
        const VkDevice Device, const VmaAllocator Allocator,
        VkBufferUsageFlags BufferUsage,
        VmaAllocationCreateFlags AllocationCreateFlags,
        VkDeviceSize Size,
        VkBuffer *pBuffer, VmaAllocation* pAllocation);

    /**
     * @brief Copies a source VkBuffer to a destination VkBuffer
     * 
     * @param Device VkDeviceHandle
     * @param CmdBuffer VkCommmandBuffer used to execute copy procedure
     * @param SrcBuffer VkBuffer source to copy from
     * @param DstBuffer VkBuffer destination to copy to
     * @param Size VkDeviceSize of the Buffer being copied
     */
    void copy_vk_buffer(
        const VkDevice Device, VkCommandBuffer CmdBuffer, 
        VkBuffer SrcBuffer, VkBuffer DstBuffer, 
        VkDeviceSize Size);


    void map_vk_allocation_data(
        VmaAllocator Allocator, VmaAllocation Allocation, 
        void* SrcData, size_t SrcDataSize);


    struct ClVkBuffer
    {
        VkBuffer        Handle;
        VmaAllocation   Allocation;
        VkDeviceSize    Size;
    };

    ClVkBuffer cl_create_vk_buffer(ClVkContext& VkCtx, VkBufferUsageFlags Usage, VmaAllocationCreateFlags VmaAllocationCreateFlags, VkDeviceSize Size);

    std::vector<ClVkBuffer> cl_create_vk_buffers(ClVkContext& VkCtx, VkBufferUsageFlags UsageFlags, VmaAllocationCreateFlags AllocationCreateFlags, VkDeviceSize Size, size_t Count);

    void cl_destroy_vk_buffer(ClVkContext& VkCtx, ClVkBuffer& Buffer);

    void cl_destroy_vk_buffers(ClVkContext& VkCtx, std::vector<ClVkBuffer> Buffers);
}

