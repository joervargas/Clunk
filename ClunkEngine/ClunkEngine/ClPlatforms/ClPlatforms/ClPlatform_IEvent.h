#pragma once

#include "ClPlatform_EventTypes.h"

namespace Clunk::Events
{
    class IEventListener
    {
    public:

    virtual ~IEventListener() {};

    };

    class IEvent
    {
    public:

        virtual ~IEvent() {};

        void Attach(IEventListener* listener);
        void Detach(IEventListener* listener);

        void Notify();

    private:

    };


    class IButtonListener : IEventListener
    {
    public:

        virtual ~IButtonListener() {}

        virtual void Execute(const EButtonState& State) = 0;

    };

    class IAxisMotionListener : IEventListener
    {
    public:

        virtual ~IAxisMotionListener() {}

        virtual void Execute(const AxisMotionData& Data) = 0;

    };


    class IWindowListener : IEventListener
    {
    public:

        virtual ~IWindowListener() {}

        virtual void Execute(const EWindowState& State) = 0;

    };
}
