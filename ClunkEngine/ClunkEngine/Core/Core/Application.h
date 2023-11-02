#pragma once

#include "Logger.h"
#include <PCH/pch.h>

// #include <WindowContext/WindowContext.h>
#include <ClPlatforms/ClPlatform.h>
#include <ClPlatforms/ClPlatform_ButtonEvent.h>
#include <ClPlatforms/ClPlatform_WindowEvent.h>
// #include <Events/WindowEvents.h>
// #include <Events/WindowListener.h>
// #include <Events/KeyEvents.h>
// #include <Events/KeyListener.h>
// #include <Events/EventDataTypes.h>

#include <ClRenderManager/ClRenderContext.h>

namespace Clunk 
{

    class Application
    {
    public:

        Application();
        virtual ~Application();

        /**
         * @brief Initializes the application
         * @return Returns true on success
         */
        virtual b8 Initialize();

        /**
         * @brief Starts the main application loop.
         * @returns True on success; otherwise false.
         */
        b8 Run();

        // /**
        //  * @brief Updates data to be rendered.
        //  * @param DeltaTime The time in seconds since last frame.
        //  * @return Returns true on success.
        //  */
        // b8 Update(f32 DeltaTime);

        /**
         * @brief Render to frame
         * @param DeltaTime The time in seconds since last frame.
         * @return Returns true on success
         */
        b8 Render(f32 DeltaTime);

        void Close(const EWindowState& State);
        // void Resize(const WindowData& Data);
        
        // void Escape(const KeyData& Data);

        void Quit() { bIsRunning = false; }

    private:

        ClPlatform* m_platformCtx;
        b8 bIsRunning;

        ClRenderContext RenderCtx;

        WindowListener<Application>* OnClose;
        // WindowListener<Application>* OnResize;
        // WindowListener<Application>* OnMaximize;
        // WindowListener<Application>* OnMinimize;
    };

    Application* CreateApplication();
}
