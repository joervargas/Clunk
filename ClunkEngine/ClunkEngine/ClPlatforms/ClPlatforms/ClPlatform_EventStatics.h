#pragma once

#include "ClPlatform_EventTypes.h"

namespace Clunk::Events
{
    namespace Key
    {
        extern ButtonEvent ESC_Key;
        extern ButtonEvent W_Key;
        extern ButtonEvent S_Key;
        extern ButtonEvent A_Key;
        extern ButtonEvent D_Key;
    } // namespace Key

    namespace Mouse
    {
        extern MouseMotionEvent MouseMotion;

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

