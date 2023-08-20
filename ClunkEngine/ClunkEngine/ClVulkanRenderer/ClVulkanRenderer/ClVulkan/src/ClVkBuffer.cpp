#include "ClVkBuffer.h"




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
        const VkDevice Device, const VkPhysicalDevice PhysDevice,
        VkBufferUsageFlags Usage, VkMemoryPropertyFlags MemProperties,
        VkDeviceSize Size,
        VkBuffer *pBuffer, VkDeviceMemory *pMemory)
    {
        const VkBufferCreateInfo buffer_info =
        {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = Size,
            .usage = Usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr,
        };
        VK_CHECK(vkCreateBuffer(Device, &buffer_info, nullptr, pBuffer));

        VkMemoryRequirements mem_requirements;
        vkGetBufferMemoryRequirements(Device, *pBuffer, &mem_requirements);

        const VkMemoryAllocateInfo alloc_info =
        {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
            .pNext = nullptr,
            .allocationSize = mem_requirements.size,
            .memoryTypeIndex = find_vk_memory_type_index(PhysDevice, mem_requirements.memoryTypeBits, MemProperties)
        };
        VK_CHECK(vkAllocateMemory(Device, &alloc_info, nullptr, pMemory));
        vkBindBufferMemory(Device, *pBuffer, *pMemory, 0);
    }

}

