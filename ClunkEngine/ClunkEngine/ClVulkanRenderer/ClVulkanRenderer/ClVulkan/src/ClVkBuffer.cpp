#include "ClVkBuffer.h"
#include "VkDefines.h"

#include <VkMemAllocator/vk_mem_alloc.h>

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
        VK_CHECK(vkCreateBuffer(Device, &buffer_info, nullptr, pBuffer));

        const VmaAllocationCreateInfo alloc_info =
        {
            .usage = VMA_MEMORY_USAGE_AUTO,
            .flags = AllocatonCreateFlags
        };
        VK_CHECK( vmaCreateBuffer(Allocator, &buffer_info, &alloc_info, pBuffer, pAllocation, nullptr) );
    }

}

