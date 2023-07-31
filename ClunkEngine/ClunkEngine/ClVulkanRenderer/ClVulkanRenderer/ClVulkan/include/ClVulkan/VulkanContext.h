#ifndef VULKAN_CONTEXT_H
#define VULKAN_CONTEXT_H

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <PCH/pch.h>

#include "VulkanLoader.h"
#include <vector>
#include <optional>

namespace Clunk::Vk
{

    struct VulkanQueue
    {
        std::optional<u32> index = std::nullopt;
        VkQueue Handle = nullptr;
    };

    struct VulkanQueues
    {
        VulkanQueue Graphics;
    };

    /**
     * @brief Queuries VkPhysicalDevice for available VkQueue family indices; populates VulkanQueues
     * 
     * @param PhysicalDevice VkPhysicalDevice handle
     * @param Queues VulkanQueues struct containing VkQueue handles and indices
     */
    void query_vulkan_queues_indices(const VkPhysicalDevice PhysicalDevice, VulkanQueues* Queues);

    /**
     * @brief Queueies VkDevice for VkQueue handles; populates VulkanQueues
     * 
     * @param Device VkDevice handle
     * @param Queues VulkanQueues struct containing VkQueue handles and indices
     */
    void query_vulkan_queues_handles(const VkDevice Device, VulkanQueues* Queues);

    /**
     * @brief Get list of all active VulkanQueue family indices
     * 
     * @param Queues VulkanQueues struct containing VkQueue handles and indices
     * @return std::vector<u32> 
     */
    std::vector<u32> get_vulkan_queues_indices_list(const VulkanQueues* Queues);

    /**
     * @brief VkSwapchin Handle, Images and ImageViews
     */
    struct VulkanSwapchain
    {
        VkSwapchainKHR Handle = nullptr;
        std::vector<VkImage> Images;
        std::vector<VkImageView> ImageViews;
        VkFormat Format;
        VkExtent2D Extent;
    };

    /**
     * @brief Create a VulkanSwapchain struct
     * 
     * @param Device VkDevice handle
     * @param PhysicalDevice VkPhysicalDevice Handle
     * @param Surface VkSurfaceKHR handle
     * @param QueueFamilyIndices u32 Queue family indices
     * @param Width Frame Width
     * @param Height Frame Height
     * @return VulkanSwapchain stores VkSwapchain handle, swapchain images and imageViews
     */
    VulkanSwapchain create_vulkan_swapchain(const VkDevice Device, VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface, std::vector<u32> QueueFamilyIndices, u32 Width, u32 Height);

    /**
     * @brief Destroy contents of VulkanSwapchain struct
     * 
     * @param Device Device Handle
     * @param pSwapchain ponter to VulkanSwapchainStruct
     */
    void destroy_vulkan_swapchain(const VkDevice Device, VulkanSwapchain* pSwapchain);


    /**
     * @brief VkCommand Pool and Buffers
     */
    struct VulkanCommands
    {
        VkCommandPool Pool;
        std::vector<VkCommandBuffer> Buffers;
    };

    /**
     * @brief Context handles in Vulkan for the current GPU device
     */
    struct VulkanContext
    {
        VkPhysicalDevice PhysicalDevice = nullptr;
        VkDevice Device = nullptr;

        VmaAllocator MemAllocator;

        VulkanQueues Queues;
        VulkanSwapchain Swapchain;

        VkSemaphore WaitSemaphore;
        VkSemaphore RenderSemaphore;

        VulkanCommands DrawCmds;
    };

    /**
     * @brief Create a VulkanContext object
     * 
     * @param Loader VulkanLoader 
     * @return VulkanContext 
     */
    VulkanContext create_vulkan_context(const VulkanLoader& Loader, u32 Width, u32 Height);

    /**
     * @brief Destroy a VulkanContext object
     * 
     */
    void destroy_vulkan_context(VulkanContext* Ctx);
}


#endif