#pragma once

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>
#include <PCH/pch.h>

#include <ClVulkan/ClVkContext.h>

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
        const VkDevice& Device, const VkCommandBuffer& CmdBuffer, 
        VkBuffer& SrcBuffer, VkBuffer& DstBuffer, 
        const VkDeviceSize Size);


    void map_vk_allocation_data(
        VmaAllocator Allocator, VmaAllocation Allocation, 
        void* SrcData, size_t SrcDataSize);


    struct ClVkBuffer
    {
        VkBuffer        Handle = nullptr;
        VmaAllocation   Allocation = nullptr;
        VkDeviceSize    Size = 0;
        u32             Count = 1;
    };

    ClVkBuffer cl_create_vk_buffer(ClVkContext& VkCtx, VkBufferUsageFlags UsageFlags, VmaAllocationCreateFlags AllocationCreateFlags, VkDeviceSize Size);

    std::vector<ClVkBuffer> cl_create_vk_buffers(ClVkContext& VkCtx, VkBufferUsageFlags UsageFlags, VmaAllocationCreateFlags AllocationCreateFlags, VkDeviceSize Size, size_t Count);

    void cl_destroy_vk_buffer(ClVkContext& VkCtx, ClVkBuffer& Buffer);

    void cl_destroy_vk_buffers(ClVkContext& VkCtx, std::vector<ClVkBuffer> Buffers);

    void cl_update_vk_buffer(ClVkContext& VkCtx, ClVkBuffer& Buffer, const void* Data, const size_t DataSize);

    template<typename T>
    ClVkBuffer cl_create_vk_uniform_buffer(ClVkContext& VkCtx, const T& Data)
    {
        VkDeviceSize data_size = sizeof(T);
        ClVkBuffer buffer = cl_create_vk_buffer(VkCtx, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT, data_size);

        return buffer;
    }

    template<typename T>
    ClVkBuffer cl_create_vk_gpu_buffer(ClVkContext& VkCtx, VkBufferUsageFlags UsageFlags, const T& Data)
    {
        VkDeviceSize data_size = sizeof(T);
        ClVkBuffer staging = cl_create_vk_buffer(VkCtx, UsageFlags | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT, data_size);

        void* data_ptr = nullptr;
        vmaMapMemory(VkCtx.MemAllocator, staging.Allocation, &data_ptr);
            memcpy(data_ptr, &Data, (size_t)data_size);
        vmaUnmapMemory(VkCtx.MemAllocator, staging.Allocation);

        ClVkBuffer buffer = cl_create_vk_buffer(VkCtx, UsageFlags | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 0, data_size);
        VkCommandBuffer cmd_buffer = cl_begin_single_time_vk_command_buffer(VkCtx);
            copy_vk_buffer(VkCtx.Device, cmd_buffer, staging.Handle, buffer.Handle, data_size);
        cl_end_single_time_vk_command_buffer(VkCtx, cmd_buffer);

        cl_destroy_vk_buffer(VkCtx, staging);

        return buffer;
    }

    template<typename T>
    ClVkBuffer cl_create_vk_gpu_array_buffer(ClVkContext& VkCtx, VkBufferUsageFlags UsageFlags, const std::vector<T>& Data)
    {
        VkDeviceSize data_size = sizeof(T) * Data.size();
        ClVkBuffer staging = cl_create_vk_buffer(VkCtx, UsageFlags | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT, data_size);

        void* data_ptr = nullptr;
        vmaMapMemory(VkCtx.MemAllocator, staging.Allocation, &data_ptr);
            memcpy(data_ptr, Data.data(), (size_t)data_size);
        vmaUnmapMemory(VkCtx.MemAllocator, staging.Allocation);

        ClVkBuffer buffer = cl_create_vk_buffer(VkCtx, UsageFlags | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 0, data_size);
        VkCommandBuffer cmd_buffer = cl_begin_single_time_vk_command_buffer(VkCtx);
            copy_vk_buffer(VkCtx.Device, cmd_buffer, staging.Handle, buffer.Handle, data_size);
        cl_end_single_time_vk_command_buffer(VkCtx, cmd_buffer);

        cl_destroy_vk_buffer(VkCtx, staging);
        
        buffer.Count = static_cast<u32>(Data.size());

        return buffer;
    }

}

