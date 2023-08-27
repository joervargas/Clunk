#pragma once

#include <vulkan/vulkan.h>
#include <ClVulkan/ClVkImg.h>
#include <ClVulkan/ClVkContext.h>
#include <ClVulkan/ClVkRenderPass.h>


namespace Clunk::Vk
{

    class ClVkRenderLayerBase
    {
    public:

        explicit ClVkRenderLayerBase(ClVkContext* pVkContex) : pVkCtx(pVkContex) {};
        
        ~ClVkRenderLayerBase();

        /*
        * @brief Returns true or false whether ClVkRenderLayerBase uses a depth buffer.
        * @return b8
        */
        inline b8 UsesDepthBuffer() { return mRenderPass.Info.bUseDepth; }

    protected:

        void BeginRenderPass(VkCommandBuffer CmdBuf, u32 CurrentImage);
        // void CreateUniformBuffers();

        ClVkContext* pVkCtx;

        VkDescriptorSetLayout mDescriptorSetLayout = nullptr;
        VkDescriptorPool mDescriptorPool = nullptr;
        std::vector<VkDescriptorSet> mDescriptorSets;

        std::vector<VkFramebuffer> mFramebuffers;
        // VkRenderPass mRenderPass = nullptr;
        ClVkRenderPass mRenderPass;

        VkPipelineLayout mPipelineLayout = nullptr;
        VkPipeline mPipeline = nullptr;

        std::vector<VkBuffer> mUniformBuffers;
        std::vector<VkDeviceMemory> mUniformBuffersDeviceMemory;
        
    };

}
