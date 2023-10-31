#pragma once

#include <TinyXML2/tinyxml2.h>

#include <PCH/pch.h>

#include <Core/Object.h>

#include "ClIRenderer.h"

#ifdef VK_GFX

    #include <ClVulkanRenderer/ClVkRenderer.h>

#endif

namespace Clunk
{
    class ClRenderContext
    {
    public:

        ClRenderContext() {};
        virtual ~ClRenderContext();

        void SetIsResized(b8 bIsResized) { m_renderer->SetIsResized(bIsResized); }
        // static ClRenderContext* GetRenderContext();
     
        void Init();
        void Update();
        void Destroy();

        void RenderFrame();

        // void ToggleFullScreen();

        // clResource* LoadResourceFromXML(tinyxml2::XMLElement* Element);
        // void RenderAllObjects();

    protected:


    private:

        ClIRenderer* m_renderer;

        #ifdef VK_GFX
            Vk::ClVkRenderer mVkRenderer;
        #endif

    };
}