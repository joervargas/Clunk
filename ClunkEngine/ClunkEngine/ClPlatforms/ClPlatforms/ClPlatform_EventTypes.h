#pragma once

#include <PCH/pch.h>


enum EButtonState : u8
{
    BS_PRESSED,
    BS_RELEASED,

    BS_MAX_DEFAULT
};

// Key / Button Event type

typedef struct DelegateButtonListener
{
    void* Obj;
    void (*fire_callback)(void* Obj, const EButtonState& State);
} DelegateButtonListener;

typedef struct ButtonEvent
{
    DelegateButtonListener* Listener;
    // void Execute(const EButtonState& State)
    // {
    //     this->Listener->fire_callback(this->Listener->Obj, State);
    // }
} ButtonEvent;


// Mouse Events
// Mouse Motion

typedef struct MouseMotionData
{
    i32 X;
    i32 Y;
    i32 X_Delta;
    i32 Y_Delta;
} MouseMotionData;

typedef struct DelegateMouseMotionListener
{
    void* Obj;
    void (*fire_callback)(void* Obj, const MouseMotionData& Data);
} DelegateMouseMotionListener;

typedef struct MouseMotionEvent
{
    DelegateMouseMotionListener* Listener;
    // void Execute(const EButtonState& State)
    // {
    //     this->Listener->fire_callback(this->Listener->Obj, State);
    // }
} MouseMotionEvent;

// Window Events

// enum EWindowState : u8
// {
//     WS_SHOWN,
//     WS_HIDDEN,
//     WS_RESIZED,
//     WS_MOVED,
//     WS_MINIMIZED,
//     WS_MAXIMIZED,
//     WS_RESTORED,
//     WS_ENTER_MOUSE_FOCUS,
//     WS_LEAVE_MOUSE_FOCUS,
//     WS_GAINED_KEY_FOCUS,
//     WS_LOST_KEY_FOCUS,
//     WS_TAKE_FOCUS,
//     WS_CLOSE,

//     WS_MAX_DEFAULT,
// };

typedef struct WindowData
{
    u32 windowID;
    // EWindowState state;
} WindowData;

typedef struct DelegateWindowListener
{
    void* Obj;
    void (*fire_callback)(void* Obj, const WindowData& Data);
} DelegateWindowListener;

typedef struct WindowEvent
{
    DelegateWindowListener* Listener;
    // void Execute(const EButtonState& State)
    // {
    //     this->Listener->fire_callback(this->Listener->Obj, State);
    // }
} WindowEvent;