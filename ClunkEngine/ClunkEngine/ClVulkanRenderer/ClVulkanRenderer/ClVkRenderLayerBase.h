#pragma once

#include <vulkan/vulkan.h>
#include <ClVulkan/ClVkImg.h>
#include <ClVulkan/ClVkContext.h>

namespace Clunk::Vk
{

    class ClVkRenderLayerBase
    {
    public:

        explicit ClVkRenderLayerBase(ClVkContext* pVkContex) : pVkCtx(pVkContex) {};
        
        ~ClVkRenderLayerBase();

    protected:

        void BeginRenderPass(VkCommandBuffer CmdBuf, u32 CurrentImage);
        // void CreateUniformBuffers();

        ClVkContext* pVkCtx;

        VkDescriptorSetLayout mDescriptorSetLayout = nullptr;
        VkDescriptorPool mDescriptorPool = nullptr;
        std::vector<VkDescriptorSet> mDescriptorSets;

        std::vector<VkFramebuffer> mFramebuffers;
        VkRenderPass mRenderPass = nullptr;

        VkPipelineLayout mPipelineLayout = nullptr;
        VkPipeline mPipeline = nullptr;

        std::vector<VkBuffer> mUniformBuffers;
        std::vector<VkDeviceMemory> mUniformBuffersDeviceMemory;
        
    };

}
