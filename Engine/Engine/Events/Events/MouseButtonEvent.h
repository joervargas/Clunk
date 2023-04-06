#pragma once

#include "IEvent.h"
#include "EventDataTypes.h"

#include <SDL2/SDL.h>
#include <list>
#include <string>

// class MouseButtonListener;

// enum EMouseButtonState : unsigned int
// {
//     MBS_PRESSED,
//     MBS_RELEASED,

//     MBS_MAX_DEFAULT
// };

class MouseButtonEvent : IEvent
{
public:

    MouseButtonData data;

    MouseButtonEvent();
    ~MouseButtonEvent();

    void Attach(IMouseButtonListener* listener);
    void Detach(IMouseButtonListener* listener);

    void Notify();

    void Set(const SDL_MouseButtonEvent& event);


private:

    std::list<IMouseButtonListener*> m_PressedListeners;
    std::list<IMouseButtonListener*> m_ReleasedListeners;
};