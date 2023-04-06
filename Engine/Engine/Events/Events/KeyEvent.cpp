#include "KeyEvent.h"
// #include "KeyListener.h"

KeyEvent::KeyEvent()
{
    
}

KeyEvent::~KeyEvent()
{
    
}

void KeyEvent::Attach(IKeyListener* listener)
{
    if(listener->State == KS_PRESSED)
    {
        m_PressedListeners.push_back(listener);
    }
    if(listener->State == KS_RELEASED)
    {
        m_ReleasedListeners.push_back(listener);
    }
}

void KeyEvent::Detach(IKeyListener* listener) 
{
    if(listener->State == KS_PRESSED)
    {
        m_PressedListeners.remove(listener);
    }
    if(listener->State == KS_RELEASED)
    {
        m_ReleasedListeners.remove(listener);
    }
}

void KeyEvent::Notify()
{
    if(data.state == KS_PRESSED)
    {
        std::list<IKeyListener*>::iterator iterator = m_PressedListeners.begin();
        while (iterator != m_PressedListeners.end())
        {
            (*iterator)->Execute(data);
            ++iterator;
        }
    }
    if(data.state == KS_RELEASED)
    {
        std::list<IKeyListener*>::iterator iterator = m_ReleasedListeners.begin();
        while (iterator != m_ReleasedListeners.end())
        {
            (*iterator)->Execute(data);
            ++iterator;
        }
        data.state = KS_MAX_DEFAULT;
    }
}

void KeyEvent::Set(const SDL_KeyboardEvent& event)
{
    data.windowID = event.windowID;
    data.timestamp = event.timestamp;
    data.repeat = event.repeat;
    data.state = event.state == SDL_PRESSED? KS_PRESSED : KS_RELEASED;

    Notify();
}
