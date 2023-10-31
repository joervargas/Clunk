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
     * @param Format VkFormat
     * @param SwapchainImages VkImages to populate
     * @param SwapchainImageViews VkImageViews to populate
     */
    void create_vk_swapchain_images(const VkDevice Device, const VkSwapchainKHR Swapchain, const VkFormat Format, std::vector<VkImage>& SwapchainImages, std::vector<VkImageView>& SwapchainImageViews);

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
     * @param Device            VkDevice handle
     * @param CommandPool       VkCommandPool handle
     * @param BufferCount       u32: Count of the command buffers to allocate.
     * @param pCommandBuffers   VkCommandBuffer* to assign to
     * 
     * @return VkResult 
     */
    VkResult allocate_vk_command_buffers(const VkDevice Device, const VkCommandPool CommandPool, u32 BufferCount, VkCommandBuffer* pCommandBuffers);

    /**
     * @brief Creates a VkSemaphore
     * 
     * @param Device VkDevice handle
     * @param pSemaphore VkSemaphore*. Populates Semaphore with this handle
     * 
     * @return VkResult 
     */
    VkResult create_vk_semaphore(VkDevice Device, VkSemaphore* pSemaphore);

    /**
     * @brief Creates a VkFence
     * @param VkDevice Device
     * @param VkFence* pFence
     * @param b8 bIsSignaled 
     * @return 
     */
    VkResult create_vk_fence(VkDevice Device, VkFence* pFence, b8 bIsSignaled = false);

    /**
     * @brief Create a VkPipelineLayout
     * 
     * @param Device                VkDevice handle
     * @param DescSetLayoutCount    u32
     * @param pDescSetLayouts       VkDescriptorSetLayout*
     * @param PushConstRangeCount   u32
     * @param pPushConstRange       VkPushConstantRange*
     * 
     * @return VkResult
    */
    VkResult create_vk_pipeline_layout(VkDevice Device, u32 DescrSetLayoutCount, VkDescriptorSetLayout* pDescSetLayouts, u32 PushConstRangeCount, VkPushConstantRange* pPushConstRange, VkPipelineLayout* pPipeLineLayout);


    /**
     * @brief Creates a VkPipelineVertexInputStateCreateInfo struct.
     * 
     * @return VkPipelineVertexInputStateCreateInfo
     */
    const VkPipelineVertexInputStateCreateInfo create_info_vk_pipeline_vertex_input();


    /**
     * @brief Creates a VkPipelineInputAssemblyStateCreateInfo struct
     * @param Topology                      VkPrimitiveTopology: Primitive Topology to draw. examples (Triangles, Lines, Points)
     * @param bPrimitiveRestartEnabled      VkBool32
     * 
     * @return VkPipelineInputAssemblyStateCreateInfo
     */
    const VkPipelineInputAssemblyStateCreateInfo create_info_vk_pipeline_assembly(VkPrimitiveTopology Topology, VkBool32 bPrimitiveRestartEnabled = VK_FALSE);

    
    /**
     * @brief Creates a VkPipelineViewportStateCreateInfo
     * @param ViewportCount     u32
     * @param pViewports        VkViewport*
     * @param ScissorCount      u32
     * @param pScissors         VkRect2D*
     * 
     * @return VkPipelineViewportStateCreateInfo
     */
    const VkPipelineViewportStateCreateInfo create_info_vk_pipeline_viewport(u32 ViewportCount, VkViewport* pViewports, u32 ScissorCount, VkRect2D* pScissors);


    /**
     * @brief Creates a VkPipelineRasterizationStateCreateInfo struct
     * 
     * @param PolygonMode   VkPolygonMode
     * @param CullMode      VkCullModeFlags
     * @param FrontFace     VkFrontFace
     * @param LineWidth     f32
     * 
     * @return VkPipelineRasterizationStateCreateInfo
     */
    const VkPipelineRasterizationStateCreateInfo create_info_vk_pipeline_rasterization(VkPolygonMode PolygonMode, VkCullModeFlags CullMode, VkFrontFace FrontFace, f32 LineWidth);


    /**
     * @brief Creates a VkPipelineMultisampleCreateInfo struct.
     * 
     * @param Samples                   VkSampleCountFlagBits
     * @param bSampleShading            VkBool32
     * @param MinSampleShading          f32
     * 
     * @return VkPipelineMultisampleStateCreateInfo
     */
    const VkPipelineMultisampleStateCreateInfo create_info_vk_pipeline_multisample(VkSampleCountFlagBits Samples, VkBool32 bSampleShading, f32 MinSampleShading = 1.0f);


    /**
     * @brief Creates a VkPipelineColorBlendAttachmentState struct
     * 
     * @param bUseBlending      b8
     * 
     * @return VkPipelineColorBlendAttachmentState
     */
    const VkPipelineColorBlendAttachmentState create_info_vk_pipeline_color_blend_attachment(b8 bUseBlending);


    /**
     * @brief Creates a VkPipelineColorBlendStateCreateInfo struct 
     * 
     * @param pColorBlendAttachments        VkPipelineColorBlendAttachmentState*
     * @param ColorBlendAttachmentsCount    u32: Number of Attachments
     * 
     * @return 
     */
    const VkPipelineColorBlendStateCreateInfo create_info_vk_pipeline_color_blend(VkPipelineColorBlendAttachmentState* pColorBlendAttachments, u32 ColorBlendAttachmentsCount);


    /**
     * @brief Creates a VkPipelineDepthStencilCreateInfo struct
     * 
     * @return VkPipelineDepthStencilCreateInfo
     */
    const VkPipelineDepthStencilStateCreateInfo create_info_vk_pipeline_depth_stencil();


    /**
     * @brief Creates a VkPipelineDynamicStateCreateInfo struct
     * 
     * @param pDynamicStates        VkDynamState*
     * @param DynamicStateCount     u32: VkDynamicState count
     * 
     * @return VkPipelineDynamicStateCreateInfo
     */
    const VkPipelineDynamicStateCreateInfo create_info_vk_pipeline_dynamic_state(VkDynamicState* pDynamicStates, u32 DynamicStateCount);


    /**
     * @brief Creates a VkPipelineTessellationStateCreateInfo struct
     * 
     * @param NumPatchControlPoints     u32
     * 
     * @return VkPipelineTessellationStateCreateInfo
     */
    const VkPipelineTessellationStateCreateInfo create_info_vk_pipeline_tessellation(u32 NumPatchControlPoints);
}

#endif