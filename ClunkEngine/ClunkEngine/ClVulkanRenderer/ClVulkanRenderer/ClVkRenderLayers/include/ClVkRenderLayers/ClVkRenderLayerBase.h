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

        virtual void DrawFrame(VkCommandBuffer& CmdBuffer, size_t CurrentImage) = 0;

    protected:

        void BeginRenderPass(VkCommandBuffer CmdBuffer, u32 CurrentImage);
        void EndRenderPass(VkCommandBuffer CmdBuffer);

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
        
    };

    class ClVk2dLayer : public ClVkRenderLayerBase
    {
    public:

        ClVk2dLayer(ClVkContext* pVkCtx) : ClVkRenderLayerBase(pVkCtx) {}

        ~ClVk2dLayer();

        virtual void Update(u32 CurrentIndex, f32 DeltaTime) = 0;

        virtual void DrawFrame(VkCommandBuffer& CmdBuffer, size_t CurrentImage) = 0;

    private:


    };

    class ClVk3dLayer : public ClVkRenderLayerBase
    {
    public:

        ClVk3dLayer(ClVkContext* pVkCtx) : ClVkRenderLayerBase(pVkCtx) {}

        ~ClVk3dLayer();

        virtual void Update(u32 CurrentIndex, f32 DeltaTime) = 0;

        virtual void DrawFrame(VkCommandBuffer& CmdBuffer, size_t CurrentImage) = 0;

    private:



    };
}
