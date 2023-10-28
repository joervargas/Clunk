#include "ClRenderContext.h"
#include <Core/Logger.h>
#include <utility>

#ifdef VK_GFX

    #include <ClVulkanRenderer/ClVkRenderer.h>

#endif

namespace Clunk
{
    ClRenderContext::~ClRenderContext()
    {
        CLUNK_DELETE(m_renderer);
    }

    void ClRenderContext::Init()
    {
        #ifdef VK_GFX
            m_renderer = new Vk::ClVkRenderer("Clunk", VK_MAKE_API_VERSION(0, 0, 0, 1));
            CLOG_INFO("Vulkan Render Strategy");
        #endif

        if(!m_renderer)
        {
            THROW_EXCEPTION("Failed to set a render strategy!");
            return;
        }
        if(!m_renderer) { CLOG_ERROR("Vulkan Render Strategy null"); }
        m_renderer->Init();
    }

    void ClRenderContext::Update()
    {
        m_renderer->Update(0.f);
    }

    void ClRenderContext::Destroy()
    {
        m_renderer->Destroy();
    }


    void ClRenderContext::RenderFrame()
    {
        m_renderer->Render();
    }
};