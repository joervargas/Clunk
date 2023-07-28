#ifndef VK_UTILS_H
#define VK_UTILS_H

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <Core/Logger.h>
#include <vector>
#include <optional>

namespace Clunk::Vk
{

    /**
     * Checks the VkResult of vulkan functions. Throws errors if found.
     */
    #define VK_CHECK(x)                                     \
        {                                                   \
            VkResult err = x;                               \
            if(err)                                         \
            {                                               \
                THROW_EXCEPTION("Vulkan Error: %s", err);   \
            }                                               \
        }            


    #endif

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

    /**
     * @brief Create a VkSwapchain object
     * 
     * @param Device VkDevice handle
     * @param PhysicalDevice VkPhysicalDevice handle
     * @param Surface VkSurfaceKHR handle
     * @param QueueFamilyIndices std::vector<u32> of queue family indices
     * @param Width u32 frame width
     * @param Height u32 frame height
     * @param pSwapchain a pointer to VkSwapchainKHR handle
     * @param bSupportScreenshots  
     */
    void create_vk_swapchain(const VkDevice Device, VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface, std::vector<u32> QueueFamilyIndices, u32 Width, u32 Height, VkSwapchainKHR *pSwapchain, bool bSupportScreenshots = false);

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
     * @brief Create a VkImageView object
     * 
     * @param Device Device handle
     * @param Image VkImage
     * @param Format VkFormat of the image
     * @param AspectFlags 
     * @param pImageView VkImageView* to create
     * @param ImageViewType VkImageViewType
     * @param LayerCount u32 Image layer count
     * @param MipLevels u32 Image mip level count
     */
    void create_vk_image_view(
        VkDevice Device, 
        VkImage Image, VkFormat Format, 
        VkImageAspectFlags AspectFlags, 
        VkImageView* pImageView, 
        VkImageViewType ImageViewType = VK_IMAGE_VIEW_TYPE_2D, 
        u32 LayerCount = 1, u32 MipLevels = 1
    );

    /**
     * @brief Creates a VkSemaphore
     * 
     * @param Device VkDevice handle
     * @param pSemaphore VkSemaphore*. Populates Semaphore with this handle
     * @return VkResult 
     */
    VkResult create_vk_semaphore(VkDevice Device, VkSemaphore* pSemaphore);

}