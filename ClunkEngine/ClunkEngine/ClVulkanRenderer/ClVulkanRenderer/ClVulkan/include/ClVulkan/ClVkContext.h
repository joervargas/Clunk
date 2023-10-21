#ifndef VULKAN_CONTEXT_H
#define VULKAN_CONTEXT_H

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <PCH/pch.h>
// #include "VkDefines.h"

#include "ClVkLoader.h"
#include <vector>
#include <optional>



namespace Clunk::Vk
{

    // Forward Declarations
    struct ClVkRenderPass;

    struct ClVkQueue
    {
        std::optional<u32> index = std::nullopt;
        VkQueue Handle = nullptr;
    };

    struct ClVkQueues
    {
        ClVkQueue Graphics;
    };

    /**
     * @brief Queuries VkPhysicalDevice for available VkQueue family indices; populates VulkanQueues
     * 
     * @param PhysicalDevice VkPhysicalDevice handle
     * @param Queues VulkanQueues struct containing VkQueue handles and indices
     */
    void cl_query_vk_queues_indices(const VkPhysicalDevice PhysicalDevice, ClVkQueues* Queues);

    /**
     * @brief Queueies VkDevice for VkQueue handles; populates VulkanQueues
     * 
     * @param Device VkDevice handle
     * @param Queues VulkanQueues struct containing VkQueue handles and indices
     */
    void cl_query_vk_queues_handles(const VkDevice Device, ClVkQueues* Queues);

    /**
     * @brief Get list of all active VulkanQueue family indices
     * 
     * @param Queues VulkanQueues struct containing VkQueue handles and indices
     * @return std::vector<u32> 
     */
    std::vector<u32> cl_get_vk_queues_indices_list(const ClVkQueues* Queues);

    /**
     * @brief VkSwapchin Handle, Images and ImageViews
     */
    struct ClVkSwapchain
    {
        VkSwapchainKHR Handle = nullptr;
        std::vector<VkImage> Images;
        std::vector<VkImageView> ImageViews;
        VkFormat Format;
        // VkExtent2D Extent;
        u32 Width;
        u32 Height;
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
    ClVkSwapchain cl_create_vk_swapchain(const VkDevice Device, VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface, std::vector<u32> QueueFamilyIndices, u32 Width, u32 Height);

    /**
     * @brief Destroy contents of VulkanSwapchain struct
     * 
     * @param Device Device Handle
     * @param pSwapchain ponter to VulkanSwapchainStruct
     */
    void cl_destroy_vk_swapchain(const VkDevice Device, ClVkSwapchain* pSwapchain);


    /**
     * @brief VkCommand Pool and Buffers
     */
    struct ClVkCommands
    {
        VkCommandPool Pool;
        std::vector<VkCommandBuffer> Buffers;
    };

    struct ClVkFrameSync
    {
        std::vector<VkSemaphore>    WaitSemaphores = {};
        std::vector<VkSemaphore>    RenderSemaphores = {};
        std::vector<VkFence>        InFlightFences = {};
    
    private:

        u32                         FramesInFlight{0};
        u32                         CurrentFrameIndex{0};

    public:

        ClVkFrameSync() {};
        ClVkFrameSync(VkDevice Device, u32 NumFramesInFlight);
        void Destroy(VkDevice Device);

        u32 GetNumFramesInFlight() { return FramesInFlight; }
        u32 GetCurrentIndex() { return CurrentFrameIndex; }
        void SetNextFrameIndex() { CurrentFrameIndex = (CurrentFrameIndex + 1) % FramesInFlight; }
        VkSemaphore GetCurrentWaitSemaphore() { return WaitSemaphores[CurrentFrameIndex]; }
        VkSemaphore GetCurrentRenderSemaphore() { return RenderSemaphores[CurrentFrameIndex]; }
        VkFence GetCurrentInFlightFence() { return InFlightFences[CurrentFrameIndex]; }
    };

    /**
     * @brief Context handles in Vulkan for the current GPU device
     */
    struct ClVkContext
    {
        VkPhysicalDevice PhysicalDevice = nullptr;
        VkDevice Device = nullptr;

        VmaAllocator MemAllocator;

        ClVkQueues Queues;
        ClVkSwapchain Swapchain;

        // VkSemaphore WaitSemaphore;
        // VkSemaphore RenderSemaphore;
        ClVkFrameSync FrameSync;

        ClVkCommands DrawCmds;
    };

    /**
     * @brief Create a ClVkContext object
     * 
     * @param Loader ClVkLoader 
     * @return ClVkContext 
     */
    ClVkContext cl_create_vk_context(const ClVkLoader& Loader, u32 Width, u32 Height);

    /**
     * @brief Destroy a ClVkContext struct and all its resources.
     * 
     * @param VkCtx* ClVkContext to destroy
     */
    void cl_destroy_vk_context(ClVkContext* VkCtx);

    /**
     * @brief Readies a single VkCommandBuffer to recieve instructions and returns it. 
     * 
     * @param VkCtx ClVkContext struct
     * @return VkCommandBuffer handle ready to recieve instructions
     */
    VkCommandBuffer cl_begin_single_time_vk_command_buffer(ClVkContext& VkCtx);

    /**
     * @brief Submits work done on a single time buffer and frees it from memory.
     * 
     * @param VkCtx ClVkContext struct
     * @param CmdBuffer VkCommandBuffer handle to submit and free.
     */
    void cl_end_single_time_vk_command_buffer(ClVkContext& VkCtx, VkCommandBuffer CmdBuffer);

    /**
     * @brief Creates a graphics capable VkPipeline handle
     * @param VkCtx 
     * @param RenderPass 
     * @param PipelineLayout 
     * @param ShaderFiles 
     * @param bDynamicScissor 
     * @param bUseBlending 
     * @param CustomWidth 
     * @param CustomHeight 
     * @param NumPatchControlPoints 
     * @return VkPipeline 
     */
    VkPipeline cl_create_vk_graphics_pipeline(
        ClVkContext& VkCtx,
        ClVkRenderPass RenderPass,
        VkPipelineLayout PipelineLayout,
        const std::vector<const char *>& ShaderFiles,
        VkPrimitiveTopology Topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        bool bDynamicScissor = false,
        bool bUseBlending = true,
        i32 CustomWidth = -1,
        i32 CustomHeight = -1,
        u32 NumPatchControlPoints = 0
    );

    std::vector<VkFramebuffer> cl_create_vk_color_depth_framebuffers(ClVkContext& VkCtx, ClVkRenderPass& RenderPass, VkImageView& DepthView);

    std::vector<VkFramebuffer> cl_create_vk_color_only_framebuffers(ClVkContext& VkCtx, ClVkRenderPass& RenderPass);

    void cl_destroy_vk_framebuffers(VkDevice Device, std::vector<VkFramebuffer>& Framebuffers);
}


#endif