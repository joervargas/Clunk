#pragma once

#include "ClPlatform_EventTypes.h"
#include "ClPlatform_IEvent.h"

namespace Clunk::Events
{
    class ButtonEvent : IEvent
    {
    public:

        // EButtonState State;

        ButtonEvent() {};
        ~ButtonEvent() {};

        void Attach(IButtonListener* Listener) { m_listener = Listener; }
        void Detach(IButtonListener* Listener) { if(m_listener == Listener) { CLUNK_DELETE(Listener); } }

        // void Set(const SDL_KeyboardEvent& event);

        virtual void Notify(const EButtonState& State) { m_listener->Execute(State); };

    private:

        IButtonListener* m_listener;
    };

    template<class T>
    class ButtonListener : public IButtonListener
    {
    public:

        typedef void(T::*ButtonListenerCallback)(const EButtonState& State); // EventCallback

        // ButtonState State;
        
        ButtonListener() :
            // State(EBS_PRESSED),
            m_event(nullptr),
            m_target(nullptr),
            m_callBack(nullptr)
        {};

        ButtonListener(ButtonEvent* BtnEvent, T* Obj, ButtonListenerCallback Callback) :
            // State(State),
            m_event(BtnEvent),
            m_target(Obj),
            m_callBack(Callback)
        {
            this->m_event->Attach(this);
        }

        virtual ~ButtonListener() {}

        virtual void Execute(const EButtonState& State) override
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

        ButtonEvent* m_event;
        T* m_target;
        ButtonListenerCallback m_callBack;

    };
}