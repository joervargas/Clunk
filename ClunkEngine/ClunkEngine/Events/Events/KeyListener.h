#pragma once

#include "IEvent.h"
#include "KeyEvent.h"
#include "EventDataTypes.h"

// class KeyEvent;
// enum EKeyState : unsigned int;

template<class T>
class KeyListener : public IKeyListener
{
public:
    typedef void(T::*KeyListenerCallback)(const KeyData& data); // EventCallback

    // EKeyState State;

    KeyListener() :
        // State(EKeyState::KS_PRESSED),
        m_event(nullptr),
        m_target(nullptr),
        m_callBack(nullptr)
    {
        State = EKeyState::KS_MAX_DEFAULT;
    }

    KeyListener(EKeyState keyState, KeyEvent* event, T* object, KeyListenerCallback callback) :
        // State(keyState),
        m_event(event),
        m_target(object),
        m_callBack(callback)
    {
        State = keyState;
        this->m_event->Attach(this);
    }


    virtual ~KeyListener()
    {}

    virtual void Execute(const KeyData& Data)
    {
        (m_target->*m_callBack)(Data);
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

    KeyEvent* m_event;
    T* m_target;
    KeyListenerCallback m_callBack;

};