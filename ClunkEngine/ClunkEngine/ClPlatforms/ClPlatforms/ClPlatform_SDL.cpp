#include "ClPlatform.h"

#ifdef PLATFORM_SDL

#include <Core/Logger.h>
// #include "ClPlatform_EventTypes.h"
// #include "ClPlatform_EventStatics.h"
// #include <Events/WindowEvents.h>
// #include <Events/KeyEvents.h>

namespace Clunk
{
    ClPlatform ClPlatform::m_Platform;

    void ClPlatform::Create_Window(String name, i32 width, i32 height)
    {
        // Initialize SDL
        if(SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
            CLOG_ERROR("SDL could not initialize! SDL Error: %s", SDL_GetError());
        }
        
        #ifdef VK_GFX
        
            CLOG_INFO("** Created a Vulkan ready widnow! **");

            m_Window = SDL_CreateWindow(
                name.c_str(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                width, 
                height, 
                SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
            );

        #else

            CLOG_INFO("** Created a window! **");

            m_Window = SDL_CreateWindow(
                name.c_str(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                width, 
                height, 
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
            );

        #endif

        if( m_Window == NULL )
        {
            CLOG_ERROR("Window could not be created! SDL Error: %s", SDL_GetError());
        }

        #define SDL_Platform
    }

    void ClPlatform::Destroy() 
    {
        SDL_DestroyWindow( m_Window );
        SDL_Quit();
    }

    void ClPlatform::PumpEvents()
    {
        while (SDL_PollEvent(&m_Event))
        {
            switch (m_Event.type)
            {
            case SDL_QUIT :
                /* code */
                break;
            case SDL_WINDOWEVENT:
                ProcessWindowEvents(m_Event.window);
                break;
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                ProcessKeyboardInput(m_Event.key);
                break;
            case SDL_MOUSEMOTION:
                ProcessMouseMotion(m_Event.motion);
                break;
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
                ProcessMouseButton(m_Event.button);
                break;
            }
        }
        
    }

    void ClPlatform::GetDrawableSize(i32 *Width, i32 *Height)
    {
        if(!Width || !Height) { THROW_EXCEPTION("Platform::GetDrawableSize Width and Height must not be null."); }
        SDL_Vulkan_GetDrawableSize(m_Window, Width, Height);
        // SDL_GetWindowSize(m_Window, Width, Height);
    }

    void ClPlatform::Wait()
    {
        SDL_WaitEvent(&m_Event);
    }

    void ClPlatform::ProcessWindowEvents(const SDL_WindowEvent &WindowEvent)
    {
        // using namespace Events;

        // const WindowData data = { .windowID = WindowEvent.windowID };
        switch (WindowEvent.event)
        {
            // case SDL_WINDOWEVENT_SHOWN:
            //     SDL_Log("~ SDL Window %d shown \n", windowEvent.windowID);
            //     break;
            case SDL_WINDOWEVENT_RESIZED:
                Events::Window::Resize.Notify(Events::EWindowState::WS_RESIZED);
                // WindowEvents::WE_Resize.Set(WindowEvent);
                // Window::Resize.Listener->fire_callback(Window::Resize.Listener->Obj, data);
                // SDL_Log(
                //     "~ SDL Window %d size changed to %d x %d \n",
                //     windowEvent.windowID,
                //     windowEvent.data1,
                //     windowEvent.data2
                // );
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                Events::Window::Minimize.Notify(Events::EWindowState::WS_MINIMIZED);
                // WindowEvents::WE_Minimize.Set(WindowEvent);
                // SDL_Log("~ SDL Window %d minimized", windowEvent.windowID);
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                Events::Window::Maximize.Notify(Events::EWindowState::WS_MAXIMIZED);
                // WindowEvents::WE_Maximize.Set(WindowEvent);
                // SDL_Log("~ SDL Window %d maximized", windowEvent.windowID);
                break;
            case SDL_WINDOWEVENT_CLOSE:
                Events::Window::Close.Notify(Events::EWindowState::WS_CLOSE);
                // WindowEvents::WE_Close.Set(WindowEvent);
                // SDL_Log("~ SDL Window %d closed \n", windowEvent.windowID);
                break;
            default:
                break;
        }
    }

    void ClPlatform::ProcessKeyboardInput(const SDL_KeyboardEvent& KeyboardEvent) 
    {
        // switch(KeyboardEvent.type)
        // {
        //     case SDL_KEYUP:
        //     case SDL_KEYDOWN:
                switch (KeyboardEvent.keysym.sym)
                {
                case SDLK_ESCAPE:
                    Events::Keys::ESC_Key.Notify( KeyboardEvent.type == SDL_KEYDOWN ? Events::EButtonState::BS_PRESSED : Events::EButtonState::BS_RELEASED);
                    break;
                case SDLK_w:
                    Events::Keys::W_Key.Notify( KeyboardEvent.type == SDL_KEYDOWN ? Events::EButtonState::BS_PRESSED : Events::EButtonState::BS_RELEASED);
                    break;
                case SDLK_s:
                    Events::Keys::S_Key.Notify( KeyboardEvent.type == SDL_KEYDOWN ? Events::EButtonState::BS_PRESSED : Events::EButtonState::BS_RELEASED);
                    break;
                case SDLK_a:
                    Events::Keys::A_Key.Notify( KeyboardEvent.type == SDL_KEYDOWN ? Events::EButtonState::BS_PRESSED : Events::EButtonState::BS_RELEASED);
                    break;
                case SDLK_d:
                    Events::Keys::D_Key.Notify( KeyboardEvent.type == SDL_KEYDOWN ? Events::EButtonState::BS_PRESSED : Events::EButtonState::BS_RELEASED);
                    break;
                case SDLK_e:
                    Events::Keys::E_Key.Notify( KeyboardEvent.type == SDL_KEYDOWN ? Events::EButtonState::BS_PRESSED : Events::EButtonState::BS_RELEASED);
                    break;
                case SDLK_q:
                    Events::Keys::Q_Key.Notify( KeyboardEvent.type == SDL_KEYDOWN ? Events::EButtonState::BS_PRESSED : Events::EButtonState::BS_RELEASED);
                    break;
                default:
                    break;
                }
        //         break;
        //     // case SDL_KEYUP:

        //         break;
        //     default:
        //         break;
        // }
    }

    void ClPlatform::ProcessMouseMotion(const SDL_MouseMotionEvent& MouseMovtionEvent)
    {
        static i32 last_x = 0;
        static i32 last_y = 0;

        Events::AxisMotionData data = Events::AxisMotionData{
            .X = MouseMovtionEvent.x,
            .Y = MouseMovtionEvent.y,
            .X_Delta = MouseMovtionEvent.x - last_x,
            .Y_Delta = MouseMovtionEvent.y - last_y
        };
        last_x = MouseMovtionEvent.x;
        last_y = MouseMovtionEvent.y;

        Events::Mouse::MouseMotion.Notify(data);
    }

    void ClPlatform::ProcessMouseButton(const SDL_MouseButtonEvent& MouseButtonEvent) 
    {
        // switch (MouseButtonEvent.type)
        // {
        // case SDL_MOUSEBUTTONUP:
        // case SDL_MOUSEBUTTONDOWN:
            switch (MouseButtonEvent.button)
            {
            case SDL_BUTTON_LEFT:
                Events::Mouse::Left_MButton.Notify(MouseButtonEvent.type == SDL_MOUSEBUTTONDOWN ? Events::EButtonState::BS_PRESSED : Events::EButtonState::BS_RELEASED);
                break;
            case SDL_BUTTON_MIDDLE:
                Events::Mouse::Middle_MButton.Notify(MouseButtonEvent.type == SDL_MOUSEBUTTONDOWN ? Events::EButtonState::BS_PRESSED : Events::EButtonState::BS_RELEASED);
                break;
            case SDL_BUTTON_RIGHT:
                Events::Mouse::Right_MButton.Notify(MouseButtonEvent.type == SDL_MOUSEBUTTONDOWN ? Events::EButtonState::BS_PRESSED : Events::EButtonState::BS_RELEASED);
                break;
            default:
                break;
            }
            // break;
        // default:
        //     break;
        // }
    }

}

#endif