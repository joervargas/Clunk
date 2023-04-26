#pragma once


#include "WindowEvent.h"
#include "IEvent.h"
#include "EventDataTypes.h"

#include <PCH/pch.h>

template<class T>
class WindowListener : public IWindowListener
{
public:

    typedef void(T::*WindowListenerCallback)(const WindowData& data); // EventCallback

    WindowListener() :
      m_event(nullptr),
      m_target(nullptr),
      m_callBack(nullptr)
    {}

    WindowListener(WindowEvent* event, T* obj, WindowListenerCallback callback) :
      m_event(event),
      m_target(obj),
      m_callBack(callback)
    {
        this->m_event->Attach(this);
    }

    virtual ~WindowListener() {}

    virtual void Execute(const WindowData& data) override
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

    WindowEvent* m_event;
    T* m_target;
    WindowListenerCallback m_callBack;

};