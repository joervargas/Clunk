#include "ClRenderContext.h"
#include <Core/Logger.h>
#include <utility>

#ifdef VK_GFX

    #include <ClVulkanRenderer/ClVkRenderer.h>

#endif

namespace Clunk
{
    void ClRenderContext::Init()
    {
        #ifdef VK_GFX
            m_renderer = &VkRenderer;
            CLOG_INFO("Vulkan Render Strategy");
        #endif

        if(!m_renderer)
        {
            CLOG_ERROR("Failed to set a render strategy!");
            return;
        }
        
        m_renderer->Init();
    }

    // void ClRenderContext::Update()
    // {
    //     m_renderer->Update();
    // }

    void ClRenderContext::Destroy()
    {
        m_renderer->Destroy();
    }

    void ClRenderContext::BeginFrame()
    {
        m_renderer->BeginFrame();
    }

    void ClRenderContext::RenderFrame()
    {
        m_renderer->RenderFrame();
    }
};