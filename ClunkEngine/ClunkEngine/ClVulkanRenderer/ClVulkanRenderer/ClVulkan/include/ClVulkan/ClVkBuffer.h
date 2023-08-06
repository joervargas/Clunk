#pragma once

#include <vulkan/vulkan.h>


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

    void create_vk_buffer(
        const VkDevice Device, const VkPhysicalDevice PhysDevice,
        VkBufferUsageFlags Usage, VkMemoryPropertyFlags MemProperties, 
        VkDeviceSize Size,
        VkBuffer* pBuffer, VkDeviceMemory* pMemory);

    void copy_vk_buffer(
        const VkDevice Device, VkCommandBuffer CmdBuffer, 
        VkBuffer SrcBuffer, VkBuffer DstBuffer, 
        VkDeviceSize Size);

}
