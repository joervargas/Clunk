#include "WindowEvent.h"



void WindowEvent::Attach(IWindowListener* listener) 
{
    m_Listeners.push_back(listener);
}

void WindowEvent::Detach(IWindowListener* listener) 
{
    m_Listeners.remove(listener);
}


void WindowEvent::Notify() 
{
    std::list<IWindowListener*>::iterator iterator = m_Listeners.begin();
    while (iterator != m_Listeners.end())
    {
        (*iterator)->Execute(data);
        ++iterator;
    }
}


void WindowEvent::Set(const SDL_WindowEvent& event)
{
    data.windowID = event.windowID;
    data.timestamp = event.timestamp;
    
    switch (event.event)
    {
    case SDL_WINDOWEVENT_NONE:
        data.state = WS_MAX_DEFAULT;
        break;
    case SDL_WINDOWEVENT_SHOWN:
        data.state = WS_SHOWN;
        break;
    case SDL_WINDOWEVENT_HIDDEN:
        data.state = WS_HIDDEN;
        break;
    case SDL_WINDOWEVENT_MOVED:
        data.state = WS_MOVED;
        break;
    case SDL_WINDOWEVENT_RESIZED:
        data.state = WS_RESIZED;
        break;
    case SDL_WINDOWEVENT_MINIMIZED:
        data.state = WS_MINIMIZED;
        break;
    case SDL_WINDOWEVENT_MAXIMIZED:
        data.state = WS_MAXIMIZED;
        break;
    case SDL_WINDOWEVENT_RESTORED:
        data.state = WS_RESTORED;
        break;
    case SDL_WINDOWEVENT_ENTER:
        data.state = WS_ENTER_MOUSE_FOCUS;
        break;
    case SDL_WINDOWEVENT_LEAVE:
        data.state = WS_LEAVE_MOUSE_FOCUS;
        break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
        data.state = WS_GAINED_KEY_FOCUS;
        break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
        data.state = WS_LOST_KEY_FOCUS;
        break;
    case SDL_WINDOWEVENT_TAKE_FOCUS:
        data.state = WS_TAKE_FOCUS;
        break;
    case SDL_WINDOWEVENT_CLOSE:
        data.state = WS_CLOSE;
        break;
    }

    Notify();
}
