#pragma once

#include "MouseButtonEvent.h"
#include "IEvent.h"
#include "EventDataTypes.h"



template<class T>
class MouseButtonListener : public IMouseButtonListener
{
public:

    typedef void(T::*MouseButtonListenerCallback)(const MouseButtonData& data); // EventCallback

    EMouseButtonState State;
    
    MouseButtonListener() :
        State(MBS_PRESSED),
        m_event(nullptr),
        m_target(nullptr),
        m_callBack(nullptr)
    {};

    MouseButtonListener(EMouseButtonState btnState, MouseButtonEvent* event, T* obj, MouseButtonListenerCallback callback) :
        State(btnState),
        m_event(event),
        m_target(obj),
        m_callBack(callback)
    {
        this->m_event->Attach(this);
    }

    virtual ~MouseButtonListener() {}

    virtual void Execute(const MouseButtonData& data)
    {
        (m_target->*m_callBack)(data);
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

    MouseButtonEvent* m_event;
    T* m_target;
    MouseButtonListenerCallback m_callBack;

};