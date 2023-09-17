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

        virtual void FillCmdBuffer(VkCommandBuffer& CmdBuffer, size_t CurrentImage) = 0;

    protected:

        void BeginRenderPass(VkCommandBuffer CmdBuf, u32 CurrentImage);
        // void CreateUniformBuffers();

        ClVkContext* pVkCtx;

        VkDescriptorSetLayout mDescSetLayout = nullptr;
        VkDescriptorPool mDescPool = nullptr;
        std::vector<VkDescriptorSet> mDescSets;


        u32 mFrameBufferWidth = 0;
        u32 mFrameBufferHeight = 0;
        std::vector<VkFramebuffer> mFramebuffers;
        ClVkRenderPass mRenderPass;

        VkPipelineLayout mPipelineLayout = nullptr;
        VkPipeline mPipeline = nullptr;

        std::vector<VkBuffer> mUniformBuffers;
        std::vector<VkDeviceMemory> mUniformBuffersDeviceMemory;
        
    };

}
