#pragma once

#include <PCH/pch.h>
#include <ClRenderManager/ClIRenderer.h>
#include <ClPlatforms/ClPlatform.h>
#include <ClMath/ClMath.h>

#include <ClVkRenderLayers/ClVkRenderLayers.h>
#include "ClVkTransforms.h"

#include <vulkan/vulkan.h>
#include <chrono>

// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>



namespace Clunk::Vk
{
    class ClVkRenderer : public ClIRenderer
    {
    public:

        ClVkRenderer() {};
        ClVkRenderer(const char* AppName, const u32 AppVersion);
        
        ClVkRenderer(const ClVkRenderer& Other) :
            mVkLoader(Other.mVkLoader), mVkCtx(Other.mVkCtx),
            mDepthImage(Other.mDepthImage),
            mWorldTransform(Other.mWorldTransform), mWorldTransformUniform(Other.mWorldTransformUniform),
            mBeginLayer(Other.mBeginLayer), mEndLayer(Other.mEndLayer),
            mLayers2d(Other.mLayers2d), mLayers3d(Other.mLayers3d),
            m_bIsResized(Other.m_bIsResized)
        {}

        ClVkRenderer(ClVkRenderer&& Other) 
        {
            mVkLoader = std::move(Other.mVkLoader);
            mVkCtx = std::move(Other.mVkCtx);
            mDepthImage = std::move(Other.mDepthImage);
            mWorldTransform = std::move(Other.mWorldTransform);
            mWorldTransformUniform = std::move(Other.mWorldTransformUniform);
            mBeginLayer = std::move(Other.mBeginLayer);
            mEndLayer = std::move(Other.mEndLayer);
            mLayers2d = std::move(Other.mLayers2d);
            mLayers3d = std::move(Other.mLayers3d);
            m_bIsResized = std::move(Other.m_bIsResized);
        }

        ClVkRenderer& operator=(const ClVkRenderer& Other)
        {
            // return *this = ClVkRenderer(Other);
            mVkLoader = Other.mVkLoader;
            mVkCtx = Other.mVkCtx;
            mDepthImage = Other.mDepthImage;
            mWorldTransform = Other.mWorldTransform;
            mWorldTransformUniform = Other.mWorldTransformUniform;
            mBeginLayer = Other.mBeginLayer;
            mEndLayer = Other.mEndLayer;
            mLayers2d = Other.mLayers2d;
            mLayers3d = Other.mLayers3d;
            m_bIsResized = Other.m_bIsResized;

            return *this;
        }

        ClVkRenderer& operator=(ClVkRenderer&& Other)
        {
            // return *this = std::move(ClVkRenderer(Other));
            mVkLoader = std::move(Other.mVkLoader);
            mVkCtx = std::move(Other.mVkCtx);
            mDepthImage = std::move(Other.mDepthImage);
            mWorldTransform = std::move(Other.mWorldTransform);
            mWorldTransformUniform = std::move(Other.mWorldTransformUniform);
            mBeginLayer = std::move(Other.mBeginLayer);
            mEndLayer = std::move(Other.mEndLayer);
            mLayers2d = std::move(Other.mLayers2d);
            mLayers3d = std::move(Other.mLayers3d);
            m_bIsResized = std::move(Other.m_bIsResized);

            return *this;
        }

        virtual ~ClVkRenderer() {};

        virtual void SetIsResized(b8 bIsResized) override { m_bIsResized = bIsResized; }
        
        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Update(f32 DeltaTime) override;
        virtual void Render() override;

        void DrawFrame(const VkCommandBuffer& DrawBuffer, const u32& ImageIndex);

    protected:

        ClVkLoader mVkLoader;

        ClVkContext mVkCtx;

        ClVkImage mDepthImage;

        ClVkTransforms mWorldTransform;
        ClVkBuffer mWorldTransformUniform;

        ClVkBeginLayer mBeginLayer;

        ClVkEndLayer mEndLayer;

        ClVk2dLayerList mLayers2d;

        ClVk3dLayerList mLayers3d;

        void CleanupSwapchain();
        void RecreateSwapchain();

    private:

        b8 m_bIsResized{false};

    };
}
