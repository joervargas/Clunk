#include "Application.h"


namespace Clunk
{

    Application::Application()
        :  bIsRunning(false)
    {
        m_platformCtx = ClPlatform::Get();
    }

    Application::~Application()
    {
        delete(OnClose);
        delete(OnResize);
        // delete(OnMaximize);
        // delete(OnMinimize);
    }

    b8 Application::Initialize() 
    {
        bIsRunning = true;
        m_platformCtx->Create_Window("Clunk Engine 1", 1000, 800);

        // RenderManager.Init();
        RenderCtx.Init();

        // Set events
        OnClose = new WindowListener<Application>(&WindowEvents::WE_Close, this, &Application::Close);
        OnResize = new WindowListener<Application>(&WindowEvents::WE_Resize, this, &Application::Resize);

        return true;
    }

    b8 Application::Run() 
    {
        while (bIsRunning)
        {
            m_platformCtx->PumpEvents();

            RenderCtx.RenderFrame();
        }

        // RenderManager.Destroy();
        RenderCtx.Destroy();
        return true;
    }

    void Application::Close(const WindowData& Data) 
    {
        CLOG_INFO("Close command executed!");
        Quit();
    }

    void Application::Resize(const WindowData &Data)
    {
        CLOG_WARN("Window Resized!");
        // RenderManager.bIsResized = true;
        RenderCtx.SetIsResized(true);
    }

    void Application::Escape(const KeyData& Data)
    {
        CLOG_INFO("Close command executed!");
        Quit();
    }
    
}
