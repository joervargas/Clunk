#pragma once

#include <vulkan/vulkan.h>
#include <PCH/pch.h>

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
     * @brief Populates VkBuffer and VkDeviceMemory handles
     * 
     * @param Device VkDevice handle
     * @param PhysDevice VkPhysicalDevice handle
     * @param Usage VkBufferUsageFlags
     * @param MemProperties VkMemoryProperties
     * @param Size VkDeviceSize ( buffer size )
     * @param pBuffer VkBuffer* handle to populate
     * @param pMemory VkDeviceMemory* handle to populate
     */
    void create_vk_buffer(
        const VkDevice Device, const VkPhysicalDevice PhysDevice,
        VkBufferUsageFlags Usage, VkMemoryPropertyFlags MemProperties, 
        VkDeviceSize Size,
        VkBuffer* pBuffer, VkDeviceMemory* pMemory);

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

}
