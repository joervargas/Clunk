#pragma once

#include <VkMemAllocator/vk_mem_alloc.h>

/**
 * @brief Handle for VkMemAllocator's VmaAllocator with wrappers for VmaAllocator related functions.
 * 
 */
class ClVkMemAllocator
{
public:

    /**
     * @brief Creates the VmaAllocator handle. Must be called after ClVkInstance, ClVkPhysicalDevice, and ClVKDevice handles have been set.
     */
    void Create();

    /**
     * @brief Destroys VmaAllocator handle.
     */
    void Destroy();

    /**
     * @brief Get a Singleton instance of ClVkMemAllocator.
     * @return ClVkMemAllocator* 
     */
    static ClVkMemAllocator* Get() { return &m_ClVkMemAllocator; }

    /**
     * @brief Handle to VmaAllocator
     * @return const VmaAllocator* 
     */
    const VmaAllocator* Handle() { return &m_allocator; }

private:

    ClVkMemAllocator() {};

    static ClVkMemAllocator m_ClVkMemAllocator;

    VmaAllocator m_allocator;

public:

    /**
     * @brief (vmaCreateBuffer wrapper) Creates VkBuffer and VmaAllocation objects
     * @param pBufferCreateInfo VkBufferCreateInfo*: Info for VkBuffer creation
     * @param pAllocationCreateInfo VmaAllocationCreateInfo*: Info for VmaAllocation creation
     * @param pBuffer VkBuffer* handle to be assigned to
     * @param pAllocation VmaAllocation* handle to be assigned to
     * @param pAllocationInfo VmaAllocationInfo TODO: find purpose...
     * @return VkResult 
     */
    VkResult CreateBuffer(const VkBufferCreateInfo* pBufferCreateInfo, const VmaAllocationCreateInfo* pAllocationCreateInfo, VkBuffer* pBuffer, VmaAllocation* pAllocation, VmaAllocationInfo* pAllocationInfo) { return vmaCreateBuffer(m_allocator, pBufferCreateInfo, pAllocationCreateInfo, pBuffer, pAllocation, pAllocationInfo); }
    
    /**
     * @brief (vmaDestroyBuffer wrapper) Destroys VkBuffer and frees VmaAllocation memory.
     * @param Buffer VkBuffer to be destroyed
     * @param Allocation VmaAllocation memory to be freed
     */
    void DestroyBuffer(VkBuffer& Buffer, VmaAllocation& Allocation) { vmaDestroyBuffer(m_allocator, Buffer, Allocation); }

    /**
     * @brief (vmaCreateImage wrapper) Create VkImage and VmaAllocation objects
     * @param pImageCreateInfo VkImageCreateInfo*: Info for VkImage creation
     * @param pAllocationCreateInfo VmaAllocationCreateInfo*: Info for VmaAllocation creation
     * @param pImage VkImage* handle to assign to
     * @param pAllocation VmaAllocation* handle to assign to
     * @param pAllocationInfo VmaAllocationInfo* TODO: find purpose...
     * @return VkResult 
     */
    VkResult CreateImage(const VkImageCreateInfo* pImageCreateInfo, const VmaAllocationCreateInfo* pAllocationCreateInfo, VkImage* pImage, VmaAllocation* pAllocation, VmaAllocationInfo* pAllocationInfo) { return vmaCreateImage(m_allocator, pImageCreateInfo, pAllocationCreateInfo, pImage, pAllocation, pAllocationInfo); }
    
    /**
     * @brief (vmaDestroyImage wrapper) Destroys VkImage and frees VmaAllocation memory
     * @param Image VkImage to be destroyed
     * @param Allocation VmaAllocation memory to be freed
     */
    void DestroyImage(VkImage& Image, VmaAllocation& Allocation) { vmaDestroyImage(m_allocator, Image, Allocation); }

    /**
     * @brief Maps memory
     * @param Allocation
     * @param pData 
     * @return VkResult 
     */
    VkResult MapMemory(VmaAllocation& Allocation, void** pData) { return vmaMapMemory(m_allocator, Allocation, pData); }

    /**
     * @brief Unmaps memory
     * @param Allocation 
     */
    void UnmapMemory(VmaAllocation& Allocation) { vmaUnmapMemory(m_allocator, Allocation); }
};