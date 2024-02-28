#pragma once

#include "ClPlatform_EventTypes.h"
#include "ClPlatform_ButtonEvent.h"
#include "ClPlatform_AxisMotionEvent.h"
#include "ClPlatform_WindowEvent.h"

namespace Clunk::Events
{
    namespace Keys
    {
        extern ButtonEvent ESC_Key;
        extern ButtonEvent W_Key;
        extern ButtonEvent S_Key;
        extern ButtonEvent A_Key;
        extern ButtonEvent D_Key;
    } // namespace Key

    namespace Mouse
    {
        extern AxisMotionEvent MouseMotion;

        extern ButtonEvent Left_MButton;
        extern ButtonEvent Middle_MButton;
        extern ButtonEvent Right_MButton;
    } // namespace Mouse
    
    namespace Window
    {
        extern WindowEvent Close;
        extern WindowEvent Resize;
        extern WindowEvent Minimize;
        extern WindowEvent Maximize;   
    } // namespace Window
    
} // namespace Clunk::Events

