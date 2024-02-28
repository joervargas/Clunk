#pragma once

#include "ClPlatform_EventTypes.h"
#include "ClPlatform_IEvent.h"


namespace Clunk::Events
{
    class AxisMotionEvent : IEvent
    {
    public:

        AxisMotionData data;

        AxisMotionEvent() {};
        ~AxisMotionEvent() {};

        void Attach(IAxisMotionListener* Listener) { m_listener = Listener; }
        void Detach(IAxisMotionListener* Listener) { if(m_listener == Listener) { CLUNK_DELETE(m_listener); } };

        void Notify(const AxisMotionData& Data) { m_listener->Execute(Data); };

        // void Set(const AxisMotionData& event);

    private:

        IAxisMotionListener* m_listener;
    };

    template<class T>
    class AxisMotionListener : public IAxisMotionListener
    {
    public:

        typedef void(T::*AxisListenerCallback)(const AxisMotionData& Data); // EventCallback

        AxisMotionListener() :
            m_event(nullptr),
            m_target(nullptr),
            m_callBack(nullptr)
        {}

        AxisMotionListener(AxisMotionEvent* AxisEvent, T* Obj, AxisListenerCallback Callback) :
            m_event(AxisEvent),
            m_target(Obj),
            m_callBack(Callback)
        {
            this->m_event->Attach(this);
        }

        virtual ~AxisMotionListener() {}

        virtual void Execute(const AxisMotionData& Data)
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

        AxisMotionEvent* m_event;
        T* m_target;
        AxisListenerCallback m_callBack;

    };
}
