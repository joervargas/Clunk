#include "ClVkBuffer.h"
#include "VkDefines.h"

#include <VkMemAllocator/vk_mem_alloc.h>

#include "ClVkContext.h"

namespace Clunk::Vk
{

    u32 find_vk_memory_type_index(const VkPhysicalDevice PhysDevice, u32 TypeFilter, VkMemoryPropertyFlags Properties)
    {
        VkPhysicalDeviceMemoryProperties mem_properties;
        vkGetPhysicalDeviceMemoryProperties(PhysDevice, &mem_properties);

        for(u32 i = 0; i < mem_properties.memoryTypeCount; i++)
        {
            if((TypeFilter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & Properties) == Properties)
            {
                return i;
            }
        }
        return 0xFFFFFFFF;
    }

    void create_vk_buffer(
        const VkDevice Device, const VmaAllocator Allocator,
        VkBufferUsageFlags BufferUsage,
        VmaAllocationCreateFlags AllocatonCreateFlags,
        VkDeviceSize Size,
        VkBuffer *pBuffer, VmaAllocation* pAllocation)
    {
        const VkBufferCreateInfo buffer_info =
        {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = Size,
            .usage = BufferUsage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr,
        };

        const VmaAllocationCreateInfo alloc_info =
        {
            .flags = AllocatonCreateFlags,
            .usage = VMA_MEMORY_USAGE_AUTO
        };
        VK_CHECK( vmaCreateBuffer(Allocator, &buffer_info, &alloc_info, pBuffer, pAllocation, nullptr) );
    }

    void copy_vk_buffer(const VkDevice &Device, const VkCommandBuffer& CmdBuffer, VkBuffer& SrcBuffer, VkBuffer& DstBuffer, const VkDeviceSize Size)
    {
        VkBufferCopy region = {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = Size
        };
        vkCmdCopyBuffer(CmdBuffer, SrcBuffer, DstBuffer, 1, &region);
    }

    void map_vk_allocation_data(VmaAllocator Allocator, VmaAllocation Allocation, void *SrcData, size_t SrcDataSize)
    {
        void* pData;
        VK_CHECK(vmaMapMemory(Allocator, Allocation, &pData));
            memcpy(pData, SrcData, SrcDataSize);
        vmaUnmapMemory(Allocator, Allocation);
    }

    ClVkBuffer cl_create_vk_buffer(ClVkContext& VkCtx, VkBufferUsageFlags UsageFlags, VmaAllocationCreateFlags AllocationCreateFlags, VkDeviceSize Size)
    {
        VkBuffer Handle = nullptr;
        VmaAllocation Allocation = nullptr;
        create_vk_buffer(VkCtx.Device, VkCtx.MemAllocator, UsageFlags, AllocationCreateFlags, Size, &Handle, &Allocation);

        return ClVkBuffer{ Handle, Allocation, Size };
    }

    std::vector<ClVkBuffer> cl_create_vk_buffers(ClVkContext& VkCtx, VkBufferUsageFlags UsageFlags, VmaAllocationCreateFlags AllocationCreateFlags, VkDeviceSize Size, size_t Count)
    {
        std::vector<ClVkBuffer> buffers{};
        for(u32 i = 0; i < Count; i++)
        {
            ClVkBuffer buffer = cl_create_vk_buffer(VkCtx, UsageFlags, AllocationCreateFlags, Size);
            buffers.push_back(buffer);
        }

        return buffers;
    }

    void cl_destroy_vk_buffer(ClVkContext& VkCtx, ClVkBuffer& Buffer)
    {
        vmaDestroyBuffer(VkCtx.MemAllocator, Buffer.Handle, Buffer.Allocation);
    }

    void cl_destroy_vk_buffers(ClVkContext& VkCtx, std::vector<ClVkBuffer> Buffers)
    {
        for(u32 i = 0; i < Buffers.size(); i++)
        {
            vmaDestroyBuffer(VkCtx.MemAllocator, Buffers[i].Handle, Buffers[i].Allocation);
        }
        Buffers.clear();
    }
}
