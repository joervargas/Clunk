#pragma once

#include <vulkan/vulkan.h>
#include <ClVulkan/ClVkBuffer.h>
#include <ClVulkan/ClVkImg.h>
#include <ClVulkan/ClVkContext.h>
#include <ClVulkan/ClVkRenderPass.h>

#include <utility>

namespace Clunk::Vk
{

    class ClVkRenderLayerBase
    {
    public:

        explicit ClVkRenderLayerBase(ClVkContext* pVkContex) : pVkCtx(pVkContex) {};
        
        ClVkRenderLayerBase(const ClVkRenderLayerBase& Other) : 
            pVkCtx(Other.pVkCtx),
            mDescSetLayout(Other.mDescSetLayout), mDescPool(Other.mDescPool), mDescSets(Other.mDescSets),
            mFramebuffers(Other.mFramebuffers), mFrameBufferWidth(Other.mFrameBufferWidth), mFrameBufferHeight(Other.mFrameBufferHeight),
            mRenderPass(Other.mRenderPass), mPipelineLayout(Other.mPipelineLayout), mPipeline(Other.mPipeline)
        {}

        ClVkRenderLayerBase(ClVkRenderLayerBase&& Other) :
            pVkCtx(std::exchange(Other.pVkCtx, nullptr))
        {
            mDescSetLayout = std::move(Other.mDescSetLayout);
            mDescPool = std::move(Other.mDescPool);
            mDescSets = std::move(Other.mDescSets);
            mFramebuffers = std::move(Other.mFramebuffers);
            mFrameBufferWidth = std::exchange(Other.mFrameBufferWidth, 0);
            mFrameBufferHeight = std::exchange(Other.mFrameBufferHeight, 0);
            mRenderPass = std::move(Other.mRenderPass);
            mPipelineLayout = std::move(Other.mPipelineLayout);
            mPipeline = std::move(Other.mPipeline);
        }

        virtual ~ClVkRenderLayerBase();

        ClVkRenderLayerBase& operator=(const ClVkRenderLayerBase& Other)
        {
            this->pVkCtx = Other.pVkCtx;
            this->mDescSetLayout = Other.mDescSetLayout;
            this->mDescPool = Other.mDescPool;
            this->mDescSets = Other.mDescSets;
            this->mFramebuffers = Other.mFramebuffers;
            this->mFrameBufferWidth = Other.mFrameBufferWidth;
            this->mFrameBufferHeight = Other.mFrameBufferHeight;
            this->mRenderPass = Other.mRenderPass;
            this->mPipelineLayout = Other.mPipelineLayout;
            this->mPipeline = Other.mPipeline;

            return *this;
        }

        ClVkRenderLayerBase& operator=(ClVkRenderLayerBase&& Other)
        {
            this->pVkCtx = std::exchange(Other.pVkCtx, nullptr);
            this->mDescSetLayout = std::move(Other.mDescSetLayout);
            this->mDescPool = std::move(Other.mDescPool);
            this->mDescSets = std::move(Other.mDescSets);
            this->mFramebuffers = std::move(Other.mFramebuffers);
            this->mFrameBufferWidth = std::exchange(Other.mFrameBufferWidth, 0);
            this->mFrameBufferHeight = std::exchange(Other.mFrameBufferHeight, 0);
            this->mRenderPass = std::move(Other.mRenderPass);
            this->mPipelineLayout = std::move(Other.mPipelineLayout);
            this->mPipeline = std::move(Other.mPipeline);

            return *this;
        }

        virtual void Destroy();

        virtual void DrawFrame(const VkCommandBuffer& CmdBuffer, size_t CurrentImage) = 0;

        virtual void CleanupFramebuffers() { cl_destroy_vk_framebuffers(pVkCtx->Device, mFramebuffers); }

        virtual void RecreateFramebuffers(const ClVkImage* pDepthImage)
        {
            if(pDepthImage) 
            { cl_create_vk_color_depth_framebuffers(*pVkCtx, mRenderPass, pDepthImage->View); }
            else 
            { cl_create_vk_color_only_framebuffers(*pVkCtx, mRenderPass); }
        }

    protected:

        void BeginRenderPass(const VkCommandBuffer& CmdBuffer, u32 CurrentImage);
        void EndRenderPass(const VkCommandBuffer& CmdBuffer);

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
        ClVk2dLayer(const ClVk2dLayer& Other) : ClVkRenderLayerBase(Other) {}
        ClVk2dLayer(ClVk2dLayer&& Other) : ClVkRenderLayerBase(Other) {}

        ClVk2dLayer& operator=(const ClVk2dLayer& Other)
        {
            ClVkRenderLayerBase::operator=(Other);
            return *this;
        }

        ClVk2dLayer& operator=(ClVk2dLayer&& Other)
        {
            ClVkRenderLayerBase::operator=(Other);
            return *this;
        }

        virtual ~ClVk2dLayer() {};

        virtual void Update(u32 CurrentIndex, f32 DeltaTime) = 0;

        virtual void DrawFrame(const VkCommandBuffer& CmdBuffer, size_t CurrentImage) = 0;

    protected:

    };

    class ClVk2dLayerList : public ClVk2dLayer
    {
    public:

        ClVk2dLayerList() : ClVk2dLayer(nullptr) {}
        ClVk2dLayerList(ClVkContext* pVkCtx) : ClVk2dLayer(pVkCtx) {}
        ClVk2dLayerList(const ClVk2dLayerList& Other) : ClVk2dLayer(Other)
        {
            mList = Other.mList;
        }
        ClVk2dLayerList(ClVk2dLayerList&& Other) : ClVk2dLayer(Other)
        {
            mList = std::move(Other.mList);
        }

        ClVk2dLayerList& operator=(const ClVk2dLayerList& Other)
        {
            ClVk2dLayer::operator=(Other);
            mList = Other.mList;
            return *this;
        }
        ClVk2dLayerList& operator=(ClVk2dLayerList&& Other)
        {
            ClVk2dLayer::operator=(Other);
            mList = std::move(Other.mList);
            return *this;
        }

        virtual ~ClVk2dLayerList()
        {
            for(ClVk2dLayer* Layer : mList)
            {
                CLUNK_DELETE(Layer);
            }
        };

        virtual void Destroy() override;

        void Push(ClVk2dLayer* Layer) { mList.push_back(Layer); }

        ClVk2dLayer* Pop()
        {
            ClVk2dLayer* layer = mList.back(); 
            mList.pop_back(); 
            return layer;
        }

        ClVk2dLayer* operator[](size_t idx) { return mList[idx]; }

        size_t Size() { return mList.size(); }

        virtual void Update(u32 CurrentIndex, f32 DeltaTime) override;

        virtual void DrawFrame(const VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

        virtual void CleanupFramebuffers() override 
        { 
            for(ClVk2dLayer* layer : mList)
            {
                layer->CleanupFramebuffers();
            }
        }

        virtual void RecreateFramebuffers(const ClVkImage* pDepthImage) override
        {
            for(ClVk2dLayer* layer : mList)
            {
                layer->RecreateFramebuffers(pDepthImage);
            }
        }

    protected:

        std::vector<ClVk2dLayer*> mList = {};

    };

    class ClVk3dLayer : public ClVkRenderLayerBase
    {
    public:

        ClVk3dLayer(ClVkContext* pVkCtx) : ClVkRenderLayerBase(pVkCtx) {}

        virtual ~ClVk3dLayer() {};

        virtual void Update(u32 CurrentIndex, ClVkBuffer& TransformUniform, ClVkImage& DepthImage, f32 DeltaTime) = 0;

        virtual void DrawFrame(const VkCommandBuffer& CmdBuffer, size_t CurrentImage) = 0;

    private:

    };

    class ClVk3dLayerList : public ClVk3dLayer
    {
    public:

        ClVk3dLayerList() : ClVk3dLayer(nullptr) {}
        ClVk3dLayerList(ClVkContext* pVkCtx) : ClVk3dLayer(pVkCtx) {}

        virtual ~ClVk3dLayerList()
        {
            for(ClVk3dLayer* Layer : mList)
            {
                CLUNK_DELETE(Layer);
            }
        }
        virtual void Destroy() override;

        void Push(ClVk3dLayer* Layer) { mList.push_back(Layer); }

        ClVk3dLayer* Pop()
        { 
            ClVk3dLayer* layer = mList.back();
            mList.pop_back();
            return layer;
        }

        ClVk3dLayer* operator[](size_t idx) { return mList[idx]; }

        size_t Size() { return mList.size(); }

        virtual void Update(u32 CurrentIndex, ClVkBuffer& TransformUniform, ClVkImage& DepthImage, f32 DeltaTime) override;

        virtual void DrawFrame(const VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

        virtual void CleanupFramebuffers() override 
        { 
            for(ClVk3dLayer* layer : mList)
            { layer->CleanupFramebuffers(); }
        }

        virtual void RecreateFramebuffers(const ClVkImage* pDepthImage) override
        {
            for(ClVk3dLayer* layer : mList)
            { layer->RecreateFramebuffers(pDepthImage); }
        }

    protected:

        std::vector<ClVk3dLayer*> mList = {};

    };

}
