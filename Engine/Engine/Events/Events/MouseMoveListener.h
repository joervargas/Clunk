#pragma once

#include "MouseMoveEvent.h"
#include "IEvent.h"
#include "EventDataTypes.h"
// #include <SDL2/SDL.h>

template<class T>
class MouseMoveListener : public IMouseMoveListener
{
public:

    typedef void(T::*MoveListenerCallback)(const MouseMotionData& data); // EventCallback

    MouseMoveListener() :
        m_event(nullptr),
        m_target(nullptr),
        m_callBack(nullptr)
    {}

    MouseMoveListener(MouseMoveEvent* event, T* obj, MoveListenerCallback callback) :
        m_event(event),
        m_target(obj),
        m_callBack(callback)
    {
        this->m_event->Attach(this);
    }

    virtual ~MouseMoveListener() {}

    virtual void Execute(const MouseMotionData& data)
    {
        (m_target->*m_callBack)(event);
    }

        void AddToList()
    {
        if(m_event)
        {
            this->m_event->Attach(this);
        }
    }
    void RemoveFromList()
    {
        if(m_event)
        {
            this->m_event->Detach(this);
        }
    }

private:

    MouseMoveEvent* m_event;
    T* m_target;
    MoveListenerCallback m_callBack;

};