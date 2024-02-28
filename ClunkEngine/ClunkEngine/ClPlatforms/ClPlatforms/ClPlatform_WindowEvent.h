#pragma once


#include "ClPlatform_EventTypes.h"
#include "ClPlatform_IEvent.h"

namespace Clunk::Events
{
    class WindowEvent : IEvent
    {
    public:

        // WindowData data;

        WindowEvent() {};
        ~WindowEvent() {};

        void Attach(IWindowListener* Listener) { m_listener = Listener; }
        void Detach(IWindowListener* Listener) { if(m_listener == Listener) { CLUNK_DELETE(m_listener); } } 

        void Notify(const EWindowState& State) { m_listener->Execute(State); }

        // void Set(const SDL_WindowEvent& event);

    private:

        IWindowListener* m_listener;

    };

    template<class T>
    class WindowListener : public IWindowListener
    {
    public:

        typedef void(T::*WindowListenerCallback)(const EWindowState& State); // EventCallback

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

        virtual void Execute(const EWindowState& State) override
        {
            (m_target->*m_callBack)(State);
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
} // Clunk
