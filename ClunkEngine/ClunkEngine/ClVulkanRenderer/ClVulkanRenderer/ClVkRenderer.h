#pragma once

#include <ClRenderManager/ClIRenderer.h>
#include <PCH/pch.h>
#include <ClPlatforms/ClPlatform.h>

#include <ClVulkan/ClVkLoader.h>
#include <ClVulkan/ClVkContext.h>
#include "ClVk3dLayer.h"
#include "ClVkModelLayer.h"

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

        ~ClVkRenderer() {};

        virtual void SetIsResized(b8 bIsResized) override { m_bIsResized = bIsResized; }
        
        virtual void Init() override;
        virtual void Destroy() override;

        virtual void ResetFrame();

        virtual void Update() override;
        virtual void Render() override;

        void RecordDrawCmdBuffer(const VkCommandBuffer& DrawBuffer, const u32& ImageIndex);

    protected:

        ClVkLoader mVkLoader;

        ClVkContext mVkCtx;

        ClVkImage mDepthImage;

        ClVk3dLayer* p3dLayer = nullptr;

    private:

        b8 m_bIsResized{false};

    };
}
