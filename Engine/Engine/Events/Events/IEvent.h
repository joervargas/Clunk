#pragma once

#include "EventDataTypes.h"


class EventListener
{
public:

 virtual ~EventListener() {};

 virtual void Execute() {};

};

class IEvent
{
public:

    virtual ~IEvent() {};

    void Attach(EventListener* listener);
    void Detach(EventListener* listener);

    void Notify();

private:

};


class IKeyListener : EventListener
{
public:

    EKeyState State;

    virtual ~IKeyListener() {}

    virtual void Execute(const KeyData& data) {};

};

class IMouseButtonListener : EventListener
{
public:

    EMouseButtonState State;

    virtual ~IMouseButtonListener() {}

    virtual void Execute(const MouseButtonData& data) {};

};


class IMouseMoveListener : EventListener
{
public:

    virtual ~IMouseMoveListener() {}

    virtual void Execute(const MouseMotionData& data) {};

};


class IWindowListener : EventListener
{
public:

    virtual ~IWindowListener() {}

    virtual void Execute(const WindowData& data) {};

};