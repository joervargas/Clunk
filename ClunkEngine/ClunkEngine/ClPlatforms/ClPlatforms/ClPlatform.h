#pragma once

#include <PCH/pch.h>

#include "ClPlatform_EventTypes.h"
#include "ClPlatform_EventStatics.h"

#ifdef PLATFORM_SDL
    #define SDL_MAIN_HANDLED
    #include <SDL.h>
    #include <SDL_vulkan.h>
#endif


namespace Clunk
{

    class ClPlatform
    {
    public:

        static ClPlatform* Get() { return &m_Platform; }
        /**
         * @brief Performs startup routines for the window
         * @param name The name of the application.
         * @param width The initial width of the main window.
         * @param height The initial height of the main window.
         */
        void Create_Window(String name, i32 width, i32 height);

        void Destroy();
        
        void PumpEvents();

        /**
         * @brief Get the Drawable Size populates Width and Height variables with drawable surface sizes
         * @param Width Drawable Width
         * @param Height Drawable Height
         */
        void GetDrawableSize(i32* Width, i32* Height);

        void Wait();
        
        // Platform platform = Platform::Linux;
        #ifdef PLATFORM_SDL
            SDL_Window* GetWindow() const { return m_Window; }
            void ProcessWindowEvents(const SDL_WindowEvent& WindowEvent);
            
            // bool shouldQuit(SDL_Event* event);

            void ProcessKeyboardInput(const SDL_KeyboardEvent& event);

            void ProcessMouseMotion(const SDL_MouseMotionEvent& event);

            void ProcessMouseButton(const SDL_MouseButtonEvent& event);
        #endif

    private:

        ClPlatform() {};
        ~ClPlatform() {};

        static ClPlatform m_Platform;

        #ifdef PLATFORM_SDL
            // Window to display
            SDL_Window* m_Window = NULL;
            
            // SDL Events fired
            SDL_Event m_Event;
        #endif

    };

}