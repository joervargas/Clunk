#pragma once

#include <Defines/Defines.h>

namespace Clunk
{
    class ClIRenderer
    {
    public:
        
        virtual void SetIsResized(b8 bIsResized) = 0;

        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;

        virtual void RenderFrame() = 0;

        // void ToggleFullScreen();

    protected:


    private:

    };
}