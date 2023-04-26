#pragma once

#include "IEvent.h"
#include "EventDataTypes.h"

#include <SDL.h>
#include <list>

class WindowEvent : IEvent
{
public:

    WindowData data;

    WindowEvent() {};
    ~WindowEvent() {};

    void Attach(IWindowListener* listener);
    void Detach(IWindowListener* listener);

    void Notify();

    void Set(const SDL_WindowEvent& event);

private:

    std::list<IWindowListener*> m_Listeners;
};