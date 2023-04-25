#include "MouseMoveEvent.h"


MouseMoveEvent::MouseMoveEvent()
{}

MouseMoveEvent::~MouseMoveEvent()
{}

void MouseMoveEvent::Attach(IMouseMoveListener* listener)
{
    m_Listeners.push_back(listener);
}

void MouseMoveEvent::Detach(IMouseMoveListener* listener)
{
    m_Listeners.remove(listener);
}

void MouseMoveEvent::Notify(const MouseMotionData& data)
{
    std::list<IMouseMoveListener*>::iterator iterator = m_Listeners.begin();
    while (iterator != m_Listeners.end())
    {
        (*iterator)->Execute(data);
        ++iterator;
    }
}

void MouseMoveEvent::Set(const SDL_MouseMotionEvent& event)
{
    data.x = event.x;
    data.y = event.y;
    data.x_rel = event.xrel;
    data.y_rel = event.yrel;
    data.timestamp = event.timestamp;
    data.windowID = event.windowID;

    Notify(data);
}

// void MouseMoveEvent::Set(int x, int y)
// {
//     this->x = x;
//     this->y = y;
//     Notify();
// }

