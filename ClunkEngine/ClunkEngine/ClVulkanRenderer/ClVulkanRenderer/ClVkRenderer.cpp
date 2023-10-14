#include "ClVkRenderer.h"


namespace Clunk::Vk
{
    ClVkRenderer::ClVkRenderer(const char* AppName, const u32 AppVersion)
    {
        ClPlatform* platform = ClPlatform::Get();
        mVkLoader = cl_create_vk_loader(platform->GetWindow(), AppName, AppVersion);
        i32 width, height;
        platform->GetDrawableSize(&width, &height);
        mVkCtx = cl_create_vk_context(mVkLoader, width, height);
    }

    void ClVkRenderer::Init()
    {

    }

    void ClVkRenderer::Update()
    {

    }

    void ClVkRenderer::Destroy()
    {
        cl_destroy_vk_context(&mVkCtx);
        cl_destroy_vk_loader(&mVkLoader);
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
