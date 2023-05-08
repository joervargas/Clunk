#pragma once

#include "IEvent.h"
#include "EventDataTypes.h"

#include <SDL.h>
#include <list>

class MouseMoveEvent : IEvent
{
public:

    MouseMotionData data;

    MouseMoveEvent();
    ~MouseMoveEvent();

    void Attach(IMouseMoveListener* listener);
    void Detach(IMouseMoveListener* listener);

    void Notify(const MouseMotionData& data);

    void Set(const SDL_MouseMotionEvent& event);

private:

    std::list<IMouseMoveListener*> m_Listeners;
};