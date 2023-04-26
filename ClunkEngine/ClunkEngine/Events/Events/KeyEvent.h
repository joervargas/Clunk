#pragma once


#include "IEvent.h"
#include "EventDataTypes.h"

#include <SDL.h>
#include <list>
#include <string>

// enum EKeyState : unsigned int
// {
//     KS_PRESSED,
//     KS_RELEASED,

//     KS_MAX_DEFAULT
// };

class KeyEvent : IEvent
{
public:

    KeyData data;

    KeyEvent();
    ~KeyEvent();

    void Attach(IKeyListener* listener);
    void Detach(IKeyListener* listener);

    void Notify();

    void Set(const SDL_KeyboardEvent& event);


private:

    std::list<IKeyListener*> m_PressedListeners;
    std::list<IKeyListener*> m_ReleasedListeners;
};