#include "ClPlatform_EventStatics.h"


namespace Clunk::Events
{
    namespace Key
    {
        ButtonEvent ESC_Key;
        ButtonEvent W_Key;
        ButtonEvent S_Key;
        ButtonEvent A_Key;
        ButtonEvent D_Key;
    } // namespace Key

    namespace Mouse
    {
        AxisMotionEvent MouseMotion;

        ButtonEvent Left_MButton;
        ButtonEvent Middle_MButton;
        ButtonEvent Right_MButton;
    } // namespace Mouse
    
    namespace Window
    {
        WindowEvent Close;
        WindowEvent Resize;
        WindowEvent Minimize;
        WindowEvent Maximize;   
    } // namespace Window
    
} // namespace Clunk::Events
