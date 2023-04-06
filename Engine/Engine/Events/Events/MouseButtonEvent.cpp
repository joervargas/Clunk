#include "MouseButtonEvent.h"


MouseButtonEvent::MouseButtonEvent()
{
    
}

MouseButtonEvent::~MouseButtonEvent()
{
    
}

void MouseButtonEvent::Attach(IMouseButtonListener* listener)
{
    if(listener->State == MBS_PRESSED)
    {
        m_PressedListeners.push_back(listener);
    }
    if(listener->State == MBS_RELEASED)
    {
        m_ReleasedListeners.push_back(listener);
    }
}

void MouseButtonEvent::Detach(IMouseButtonListener* listener)
{
    if(listener->State == MBS_PRESSED)
    {
        m_PressedListeners.remove(listener);
    }    
    if(listener->State == MBS_RELEASED)
    {
        m_ReleasedListeners.remove(listener);
    }
}

void MouseButtonEvent::Notify()
{
    if(data.state == MBS_PRESSED)
    {
        std::list<IMouseButtonListener*>::iterator iterator = m_PressedListeners.begin();
        while (iterator != m_PressedListeners.end())
        {
            (*iterator)->Execute(data);
            ++iterator;
        }
    }
    if(data.state == MBS_RELEASED)
    {
        std::list<IMouseButtonListener*>::iterator iterator = m_ReleasedListeners.begin();
        while (iterator != m_ReleasedListeners.end())
        {
            (*iterator)->Execute(data);
            ++iterator;
        }
        data.state = MBS_MAX_DEFAULT;
    }
}

void MouseButtonEvent::Set(const SDL_MouseButtonEvent& event)
{
    data.windowID = event.windowID;
    data.timestamp = event.timestamp;
    data.x = event.x;
    data.y = event.y;
    data.clickCount = event.clicks;
    data.state = event.state == SDL_PRESSED ? MBS_PRESSED : MBS_RELEASED;

    Notify();
}
