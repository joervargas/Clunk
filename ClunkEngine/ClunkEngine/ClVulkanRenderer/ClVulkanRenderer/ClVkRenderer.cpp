#include "ClVkRenderer.h"


namespace Clunk::Vk
{
    ClVkRenderer::ClVkRenderer(const char* AppName, const u32 AppVersion)
    {
        ClPlatform* platform = ClPlatform::Get();
        mVkLoader = create_vulkan_loader(platform->GetWindow(), AppName, AppVersion);
        i32 width, height;
        platform->GetDrawableSize(&width, &height);
        mVkCtx = create_vulkan_context(mVkLoader, width, height);
    }

    void ClVkRenderer::Init()
    {

    }

    void ClVkRenderer::Update()
    {

    }

    void ClVkRenderer::Destroy()
    {

    }

    void ClVkRenderer::ResetFrame()
    {

    }

    void ClVkRenderer::Render()
    {

    }

    void ClVkRenderer::RecordDrawCmdBuffer(const VkCommandBuffer &DrawBuffer, const u32 &ImageIndex)
    {

    }
}