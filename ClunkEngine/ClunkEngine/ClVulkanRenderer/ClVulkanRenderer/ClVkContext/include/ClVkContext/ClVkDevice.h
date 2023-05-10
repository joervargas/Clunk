#pragma once

// #include <ClVkLoader/ClVkLoader.h>

#include <vulkan/vulkan.h>
#include <PCH/pch.h>

const std::vector<const char*> DeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME
    // VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
};

// PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
// VkPhysicalDevicePushDescriptorPropertiesKHR pushDescriptorProps{};

/**
 * @brief Handle to VkDevice, with wrappers for Device related functions.
 */
class ClVkDevice
{
public:

    /**
     * @brief Creates the VkDevice Handle
     */
    void Create();

    /**
     * @brief Destroys the VkDevice Handle
     */
    void Destroy();

    /**
     * @brief Returns the Singleton instance of ClVkDevice
     * @return ClVkDevice* 
     */
    static ClVkDevice* Get() { return &m_ClVkDevice; }

    /**
     * @brief Handle to VkDevice type
     * @return VkDevice* 
     */
    VkDevice* Handle() { return &m_device; }

private:

    ClVkDevice() {};

    static ClVkDevice m_ClVkDevice;
    VkDevice m_device;

public:

    /**
     * @brief (vkAquireNextImageKHR wrapper) Aquires the Next Image in the Swapchain. 
     * @param Swapchain VkSwapchainKHR
     * @param Timeout Timeout in nanoseconds for an image to become available
     * @param Semaphore Signaled when presentation engine is finished using an image
     * @param Fence Signaled when presentation engine is finished using an image
     * @param pImageIndex u32 handle to the next index of images in the Swapchain.
     * @return VkResult 
     */
    VkResult AquireNextImageKHR(const VkSwapchainKHR& Swapchain, u64 Timeout, VkSemaphore Semaphore, VkFence Fence, u32* pImageIndex) { return vkAcquireNextImageKHR(m_device, Swapchain, Timeout, Semaphore, Fence, pImageIndex); }

    /**
     * @brief (vkAllocateCommandBuffers wrapper). Allocates command buffers from the command pool.
     * @param pAllocInfo VkCommandBufferAllocateInfo
     * @param pCmdBuffers VkCommandBuffer pointer handles
     * @return VkResult 
     */
    VkResult AllocateCmdBuffers(const VkCommandBufferAllocateInfo* pAllocInfo, VkCommandBuffer* pCmdBuffers) { return vkAllocateCommandBuffers(m_device, pAllocInfo, pCmdBuffers); }

    /**
     * @brief (vkFreeCommandBuffers wrapper) Frees VkCommandBuffer memory from VkCommandPool
     * @param CmdPool The VkCommandPool which VkCommand(s) were allocated from.
     * @param CmdBufferCount Length of pCmdBuffers array.
     * @param pCmdBuffers array of VkCommandBuffer handles to be allocated to.
     */
    void FreeCmdBuffers(const VkCommandPool& CmdPool, u32 CmdBufferCount, const VkCommandBuffer* pCmdBuffers) { vkFreeCommandBuffers(m_device, CmdPool, CmdBufferCount, pCmdBuffers); }

    /**
     * @brief (vkCreateBuffer wrapper) Creates a VkBuffer object
     * @param pCreateInfo VkBufferCreateInfo: The info for Buffer Creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pBuffer VkBuffer handle to assign to.
     * @return VkResult 
     */
    VkResult CreateBuffer(VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) { return vkCreateBuffer(m_device, pCreateInfo, pAllocator, pBuffer); }

    /**
     * @brief (vkDestroyBuffer wrapper) Destroy VkBuffer object
     * @param Buffer VkBuffer to destroy
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyBuffer(VkBuffer& Buffer, const VkAllocationCallbacks* pAllocator) { vkDestroyBuffer(m_device, Buffer, pAllocator); }

    /**
     * @brief (vkCreateCommandPool wrapper) Create a VkCommandPool object
     * @param pCreateInfo VkCommandPoolCreateInfo*: The info for VkCommandPool creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pCommandPool VkCommandPool handle to assign to
     * @return VkResult 
     */
    VkResult CreateCmdPool(const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) { return vkCreateCommandPool(m_device, pCreateInfo, pAllocator, pCommandPool); }

    /**
     * @brief (vkDestroyCommandPool wrapper) Destroy a VkCommandPool object
     * @param CommandPool VkCommandPool to be destroyed
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyCmdPool(VkCommandPool& CommandPool, const VkAllocationCallbacks* pAllocator) { vkDestroyCommandPool(m_device, CommandPool, pAllocator); }

    /**
     * @brief (vkCreateDescriptorSetLayout wrapper). Create a Descriptor Set Layout object
     * @param pCreateInfo VkDescriptorSetLayoutCreateInfo*: The info for VkDescriptorSetLayout creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pSetLayout VkDescriptorSetLayout* handle to assign to
     * @return VkResult 
     */
    VkResult CreateDescSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) { return vkCreateDescriptorSetLayout(m_device, pCreateInfo, pAllocator, pSetLayout); }

    /**
     * @brief (vkDestroyDescriptorSetLayout wrapper) Destroys a VkDescriptorSetLayout object
     * @param SetLayout VkDescriptorSetLayout to destroy
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyDescSetLayout(VkDescriptorSetLayout& SetLayout, const VkAllocationCallbacks* pAllocator) { vkDestroyDescriptorSetLayout(m_device, SetLayout, pAllocator); }
    
    /**
     * @brief (vkAllocateDescriptorSets wrapper) Allocates VkDescriptorSets on the pool
     * @param pAllocateInfo VkDescriptorSetAllocateInfo*: The info for VkDescriptorSetAllocation
     * @param pDescSets VkDescriptorSet* pointer handle to assign to. Can be an array
     * @return VkResult 
     */
    VkResult AllocateDescSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescSets) { return vkAllocateDescriptorSets(m_device, pAllocateInfo, pDescSets); }

    /**
     * @brief (vkUpdateDescriptorSets wrapper)
     * @param DescWriteCount u32 
     * @param pDescWrites VkWriteDescriptorSet* handles
     * @param DescCopyCount u32
     * @param pDescCopies VkCopyDescriptorSet* handles
     */
    void UpdateDescSets(u32 DescWriteCount, const VkWriteDescriptorSet* pDescWrites, u32 DescCopyCount, const VkCopyDescriptorSet* pDescCopies) { return vkUpdateDescriptorSets(m_device, DescWriteCount, pDescWrites, DescCopyCount, pDescCopies); }

    /**
     * @brief (vkCreateDescriptorPool wrapper) Create a VkDescriptorPool object
     * @param pCreateInfo VkDescriptorPoolCreateInfo*: The info for VkDescriptorPool creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pDescriptorPool VkDescriptorPool* handle to assign to
     * @return VkResult 
     */
    VkResult CreateDescPool(const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) { return vkCreateDescriptorPool(m_device, pCreateInfo, pAllocator, pDescriptorPool); }

    /**
     * @brief (vkDestroyDescriptorPool wrapper)
     * @param DescriptorPool VkDescriptorPool to destroy.
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyDescPool(VkDescriptorPool& DescriptorPool, const VkAllocationCallbacks* pAllocator) { vkDestroyDescriptorPool(m_device, DescriptorPool, pAllocator); }
    
    /**
     * @brief (vkCreateFramebuffer wrapper) Create a VkFramebuffer object
     * @param pCreateInfo VkFramebufferCreateInfo*: The info for VkFramebuffer creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pFramebuffer VkFramebuffer* handle to assign to
     * @return VkResult 
     */
    VkResult CreateFramebuffer(const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) { return vkCreateFramebuffer(m_device, pCreateInfo, pAllocator, pFramebuffer); }

    /**
     * @brief (vkDestroyFramebuffer wrapper) Destroy a VkFramebuffer object
     * @param Framebuffer VkFramebuffer to be destroyed
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyFramebuffer(VkFramebuffer& Framebuffer, const VkAllocationCallbacks* pAllocator) { vkDestroyFramebuffer(m_device, Framebuffer, pAllocator); }

    /**
     * @brief (vkCreateFence wrapper) Create a VkFence object for syncing CPU and GPU operations
     * @param pCreateInfo VkFenceCreateInfo*: The info for VkFence creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pFence VkFence* handle to assign to
     * @return VkResult 
     */
    VkResult CreateFence(const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) { return vkCreateFence(m_device, pCreateInfo, pAllocator, pFence); }
    
    /**
     * @brief (vkDestroyFence wrapper) Destroy a VkFence object.
     * @param Fence VkFence to be destroyed
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyFence(VkFence& Fence, const VkAllocationCallbacks* pAllocator) { vkDestroyFence(m_device, Fence, pAllocator); }

    /**
     * @brief (vkCreateImage wrapper) Create a VkImage object
     * @param pCreateInfo VkImageCreateInfo*: The info for VkImage creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pImage VkImage* handle to assign to
     * @return VkResult 
     */
    VkResult CreateImage(VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) { return vkCreateImage(m_device, pCreateInfo, pAllocator, pImage); }
    
    /**
     * @brief (vkDestroyImage wrapper) Destroy a VkImage object
     * @param Image VkImage to destroy
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyImage(VkImage& Image, const VkAllocationCallbacks* pAllocator) { vkDestroyImage(m_device, Image, pAllocator); }

    /**
     * @brief (vkImageViewCreateInfo wrapper) Create a VkImageView object
     * @param pCreateInfo VkImageViewCreateInfo: The info for VkImageView creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pImageView VkImageView* handle to assign to
     * @return VkResult 
     */
    VkResult CreateImageView(VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pImageView) { return vkCreateImageView(m_device, pCreateInfo, pAllocator, pImageView); }
    
    /**
     * @brief (vkDestroyImageView wrapper) Destroy a VkImageView object
     * @param ImageView VkImageView to be destroyed
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyImageView(VkImageView& ImageView, const VkAllocationCallbacks* pAllocator) { vkDestroyImageView(m_device, ImageView, pAllocator); }

    
    /**
     * @brief (vkCreatePipelineLayout wrapper)Create a VkPiplineLayout object
     * @param pCreateInfo VkPipelineLayoutCreateInfo*: The info for VkPipelineLayout creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pPipelineLayout VkPipelineLayout* handle to assign to
     * @return VkResult 
     */
    VkResult CreatePiplineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) { return vkCreatePipelineLayout(m_device, pCreateInfo, pAllocator, pPipelineLayout); }
    
    /**
     * @brief (vkDestroyPipelineLayout wrapper) Destroy a VkPipelineLayout object
     * @param PipelineLayout VkPipelineLayout to destroy
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyPipelineLayout(VkPipelineLayout& PipelineLayout, const VkAllocationCallbacks* pAllocator) { vkDestroyPipelineLayout(m_device, PipelineLayout, nullptr); }

        /**
     * @brief (vkCreateGraphicsPipeline wrapper) Create a Graphical VkPipeline object
     * @param PipelineCache VK_NULL_HANDLE indicates caching disabled; else a handle to VkPipelineCache for saving Pipeline contents
     * @param CreateInfoCount Length of the pCreateInfos and pPipelines arrays
     * @param pCreateInfos array VkPipelineCreateInfo
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pPipelines array of VkPipeline handles to assign to
     * @return VkResult 
     */
    VkResult CreateGraphicsPipeline(VkPipelineCache PipelineCache, u32 CreateInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) { return vkCreateGraphicsPipelines(m_device, PipelineCache, CreateInfoCount, pCreateInfos, pAllocator, pPipelines); }
    
    /**
     * @brief (vkDestroyPipeline wrapper) Destroys VkPipeline object
     * @param Pipeline VkPipeline to be destroyed
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyPipeline(VkPipeline& Pipeline, const VkAllocationCallbacks* pAllocator) { vkDestroyPipeline(m_device, Pipeline, pAllocator); }

    /**
     * @brief (vkCreateRenderPass wrapper) Create a VkRenderPass object
     * @param pCreateInfo VkRenderPassCreateInfo*: The info for VkRenderPass creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pRenderPass VkRenderPass* handle to assign to
     * @return VkResult 
     */
    VkResult CreateRenderPass(const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) { return vkCreateRenderPass(m_device, pCreateInfo, pAllocator, pRenderPass); }
    
    /**
     * @brief (vkDestroyRenderPass wrapper) Destroys a VkRenderPass object
     * @param RenderPass VkRenderPass to destroy
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyRenderPass(VkRenderPass& RenderPass, const VkAllocationCallbacks* pAllocator) { vkDestroyRenderPass(m_device, RenderPass, pAllocator); }

    /**
     * @brief (vkCreateSampler wrapper) Create a VkSampler object
     * @param pCreateInfo VkSamplerCreateInfo*: The info to create VkSampler
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pSampler VkSampler* handle to assign to
     * @return VkResult 
     */
    VkResult CreateSampler(const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) { return vkCreateSampler(m_device, pCreateInfo, pAllocator, pSampler); }

    /**
     * @brief (vkDestroySampler wrapper) Destroys a VkSampler object
     * @param Sampler VkSampler to destroy
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroySampler(const VkSampler& Sampler, const VkAllocationCallbacks* pAllocator) { vkDestroySampler(m_device, Sampler, pAllocator); }

    /**
     * @brief (vkCreateSemaphore wrapper) Create a VkSemaphore object
     * @param pCreateInfo VkSemaphoreCreateInfo*: The info for VkSemaphore creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pSemaphore VkSemaphore* handle to assign to
     * @return VkResult 
     */
    VkResult CreateSemaphore(const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) { return vkCreateSemaphore(m_device, pCreateInfo, pAllocator, pSemaphore); }
    
    /**
     * @brief (vkDestroySemaphore wrapper) Destroy a VkSemaphore object
     * @param Semaphore VkSemaphore to be destroyed
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroySemaphore(VkSemaphore& Semaphore, const VkAllocationCallbacks* pAllocator) { vkDestroySemaphore(m_device, Semaphore, pAllocator); }

    /**
     * @brief (vkCreateShaderModule wrapper) Create a VkShaderModule object
     * @param pCreateInfo VkShaderModuleCreateInfo*: The info for VkShaderModule creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pShaderModule VkShaderModule* handle to assign to
     * @return VkResult 
     */
    VkResult CreateShaderModule(const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) { return vkCreateShaderModule(m_device, pCreateInfo, pAllocator, pShaderModule); }
    
    /**
     * @brief (vkDestroyShaderModule wrapper) Destroys a VkShaderModule object
     * @param ShaderModule VkShaderModule to destroy
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroyShaderModule(VkShaderModule& ShaderModule, const VkAllocationCallbacks* pAllocator) { vkDestroyShaderModule(m_device, ShaderModule, pAllocator); }

    /**
     * @brief (vkCreateSwapchainKHR wrapper) Create a SwapchainKHR object
     * @param pCreateInfo VkSwapchainCreateInfo*: The info for VkSwapchainKHR creation
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     * @param pSwapchain VkSwapchainKHR handle to assign to
     * @return VkResult 
     */
    VkResult CreateSwapchainKHR(VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) { return vkCreateSwapchainKHR(m_device, pCreateInfo, pAllocator, pSwapchain); }
    
    /**
     * @brief (vkDestroySwapchainKHR wrapper) Destroys a VkSwapchainKHR object
     * @param Swapchain VkSwapchainKHR to destroy
     * @param pAllocator VkAllocationCallbacks*: Allows assigning callbacks to memory allocation
     */
    void DestroySwapchainKHR(VkSwapchainKHR& Swapchain, VkAllocationCallbacks* pAllocator) { vkDestroySwapchainKHR(m_device, Swapchain, pAllocator); } 

    /**
     * @brief (vkDeviceWaitIdle wrapper) To wait on the host for the completion of outstanding queue operations for all queues on a given logical device, call:
     * @return VkResult 
     */
    VkResult WaitIdle() { return vkDeviceWaitIdle(m_device); }

    /**
     * @brief (vkWaitForFences wrapper) Wait for one or more fences to become signaled
     * @param FenceCount Number of VkFence(s) to wait on.
     * @param pFences An array of VkFence handles
     * @param bWaitAll If true, wait on all VkFence(s) to become signaled, else, wait on at least 1.
     * @param Timeout Timeout period in nanoseconds.
     * @return VkResult 
     */
    VkResult WaitForFences(u32 FenceCount, const VkFence* pFences, VkBool32 bWaitAll, u64 Timeout) { return vkWaitForFences(m_device, FenceCount, pFences, bWaitAll, Timeout); }

    /**
     * @brief (vkResetFences wrapper) Resets VkFence(s) to an unsignaled state
     * @param FenceCount Number of VkFence(s) to reset
     * @param pFences An array of VkFence handles
     * @return VkResult 
     */
    VkResult ResetFences(u32 FenceCount, const VkFence* pFences) { return vkResetFences(m_device, FenceCount, pFences); }
};