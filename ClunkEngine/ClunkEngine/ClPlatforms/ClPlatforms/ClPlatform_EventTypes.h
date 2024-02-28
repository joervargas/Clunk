#pragma once

#include <PCH/pch.h>

namespace Clunk::Events
{
    // Key / Button Event type
    
    enum EButtonState : u8
    {
        BS_PRESSED,
        BS_RELEASED,

        BS_MAX_DEFAULT
    };


    // Mouse Events
    // Mouse Motion

    typedef struct AxisMotionData
    {
        i32 X;
        i32 Y;
        i32 X_Delta;
        i32 Y_Delta;
    } AxisMotionData;

    // Window Events

    enum EWindowState : u8
    {
        WS_SHOWN,
        WS_HIDDEN,
        WS_RESIZED,
        WS_MOVED,
        WS_MINIMIZED,
        WS_MAXIMIZED,
        WS_RESTORED,
        WS_ENTER_MOUSE_FOCUS,
        WS_LEAVE_MOUSE_FOCUS,
        WS_GAINED_KEY_FOCUS,
        WS_LOST_KEY_FOCUS,
        WS_TAKE_FOCUS,
        WS_CLOSE,

        WS_MAX_DEFAULT,
    };

}