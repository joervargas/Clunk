#pragma once

#include <Defines/Defines.h>


typedef struct MouseMotionData
{
    u32 windowID;
    u32 timestamp;
    i32 x;
    i32 y;
    i32 x_rel;
    i32 y_rel;
} MouseMotionData;


enum EMouseButtonState : u8
{
    MBS_PRESSED,
    MBS_RELEASED,

    MBS_MAX_DEFAULT
};

typedef struct MouseButtonData
{
    u32 windowID;
    u32 timestamp;
    u8 clickCount;
    EMouseButtonState state;
    i32 x;
    i32 y;
} MouseButtonData;

enum EKeyState : u8
{
    KS_PRESSED,
    KS_RELEASED,

    KS_MAX_DEFAULT
};

typedef struct KeyData
{
    u32 windowID;
    u32 timestamp;
    u8 repeat;
    EKeyState state;
} KeyData;


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

typedef struct WindowData
{
    u32 windowID;
    u32 timestamp;
    EWindowState state;
} WindowData;