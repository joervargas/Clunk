#ifndef VK_UTILS_H
#define VK_UTILS_H

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <Core/Logger.h>
#include "VkDefines.h"

#include <vector>
#include <optional>

namespace Clunk::Vk
{   

    /**
     * @brief Determines whether the given VkPhysicalDevice is suitable for engine use cases.
     * 
     * @param Surface VkSurfaceKHR handle
     * @param PhysicalDevice VkPhysicalDevice handle
     * @return bool: True if PhysicalDevice is suitable
     */
    bool is_vk_physical_device_suitable(VkSurfaceKHR Surface, VkPhysicalDevice PhysicalDevice);

    /**
     * @brief 
     * 
     * @param Instance 
     * @param Surface 
     * @param pPhysicalDevice 
     * @return VkResult 
     */
    VkResult find_suitable_vk_physical_device(const VkInstance Instance, VkSurfaceKHR Surface, VkPhysicalDevice* pPhysicalDevice);

    /**
     * @brief Queuries VkPhysicalDevice for queue family indices
     * 
     * @param PhysicalDevice VkPhysicalDevice handle
     * @param DesiredQueueFlags Flags to identify the desired VkQueue
     * @return u32 
     */
    std::optional<u32> find_vk_queue_family_index(VkPhysicalDevice PhysicalDevice, VkQueueFlags DesiredQueueFlags);

    /**
     * @brief Create a VkDevice object
     * 
     * @param PhysicalDevice VkPhysicalDevice handle
     * @param QueueFamilyIndices std::vector<u32> Contains a list of desired queue family indices
     * @param pDevice a pointe to a VkDevice handle 
     */
    void create_vk_device(VkPhysicalDevice PhysicalDevice, std::vector<u32> QueueFamilyIndices, VkDevice* pDevice);

    /**
     * @brief Create a VkMemAllocator handle. Used for buffer and image allocation.
     * 
     * @param Instance VkInstance handle
     * @param Vk_API_Version u32 Vulkan API Version
     * @param PhysicalDevice VkPhysicalDevice handle
     * @param Device VkDevice handle
     * @param pMemAllocator VmaAllocator* handle to assign to
     */
    void create_vk_mem_allocator(const VkInstance Instance, u32 Vk_API_Version, const VkPhysicalDevice PhysicalDevice, const VkDevice Device, VmaAllocator* pMemAllocator );

    struct VkSwapchainDetails
    {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

    VkSwapchainDetails query_vk_swapchain_details(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface);

    VkSurfaceFormatKHR choose_vk_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& AvailableFormats);

    VkPresentModeKHR choose_vk_swap_present_mode(const std::vector<VkPresentModeKHR>& AvailablePresentModes);

    u32 choose_vk_swapchain_image_count(const VkSurfaceCapabilitiesKHR& capabilities);

    /**
     * @brief Create a VkSwapchainKHR handle
     * 
     * @param Device VkDevice handle
     * @param Surface VkSurfaceKHR handle
     * @param QueueFamilyIndices u32 list of queue family indices
     * @param Capabilities VkSurfaceCapabilitiesKHR
     * @param Format VkSurfaceFormatKHR
     * @param PresentMode VkPresentModeKHR
     * @param Extent Height and Width extent
     * @param pSwapchain VkSwapchain* handle to assign to
     * @param bSupportScreenshots bool Support for screenshots? TODO: functionality
     */
    void create_vk_swapchain(const VkDevice Device, VkSurfaceKHR Surface, std::vector<u32> QueueFamilyIndices, VkSurfaceCapabilitiesKHR Capabilities, VkSurfaceFormatKHR Format, VkPresentModeKHR PresentMode, u32 Width, u32 Height, VkSwapchainKHR *pSwapchain, bool bSupportScreenshots = false);

    /**
     * @brief Create  VKSwapchain Images and ImageViews
     * 
     * @param Device Device Handle
     * @param Swapchain VkSwapchain Handle
     * @param SwapchainImages VkImages to populate
     * @param SwapchainImageViews VkImageViews to populate
     */
    void create_vk_swapchain_images(const VkDevice Device, const VkSwapchainKHR Swapchain, std::vector<VkImage>& SwapchainImages, std::vector<VkImageView>& SwapchainImageViews);

    /**
     * @brief Create a VkCommandPool handle
     * 
     * @param Device VkDevice handle
     * @param QueueFamilyIndex u32 queue family index to pool from.
     * @param pCommandPool VkCommandPool* handle to assing to.
     * @return VkResult 
     */
    VkResult create_vk_command_pool(const VkDevice Device, u32 QueueFamilyIndex, VkCommandPool* pCommandPool);

    /**
     * @brief Allocates VkCommandBuffer(s) from the given VkCommandPool
     * 
     * @param Device VkDevice handle
     * @param CommandPool VkCommandPool handle
     * @param BufferCount u32 count of the command buffers to allocate
     * @param CommandBuffers std::vector<VkCommandBuffer> to assign to.
     * @return VkResult 
     */
    VkResult allocate_vk_command_buffers(const VkDevice Device, const VkCommandPool CommandPool, u32 BufferCount, std::vector<VkCommandBuffer>& CommandBuffers);

    /**
     * @brief Creates a VkSemaphore
     * 
     * @param Device VkDevice handle
     * @param pSemaphore VkSemaphore*. Populates Semaphore with this handle
     * @return VkResult 
     */
    VkResult create_vk_semaphore(VkDevice Device, VkSemaphore* pSemaphore);

    VkResult create_vk_pipeline_layout(VkDevice Device, u32 DescriptorSetLayoutCount, VkDescriptorSetLayout* pDescriptorSetLayouts, u32 PushConstantRangeCount, VkPushConstantRange* pPushConstantRange, VkPipelineLayout* pPipeLineLayout);
}

#endif