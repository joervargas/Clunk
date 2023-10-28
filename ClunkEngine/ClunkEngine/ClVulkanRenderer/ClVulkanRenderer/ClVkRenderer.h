#pragma once

#include <ClRenderManager/ClIRenderer.h>
#include <PCH/pch.h>
#include <ClPlatforms/ClPlatform.h>

#include <ClVkRenderLayers/ClVkRenderLayers.h>

#include <vulkan/vulkan.h>

// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#include <ClMath/ClMath.h>

namespace Clunk::Vk
{
    class ClVkRenderer : public ClIRenderer
    {
    public:

        ClVkRenderer() {};
        ClVkRenderer(const char* AppName, const u32 AppVersion);

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
