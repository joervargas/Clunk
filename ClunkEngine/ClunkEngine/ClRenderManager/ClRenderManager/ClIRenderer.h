#pragma once

#include <PCH/pch.h>

namespace Clunk
{
    class ClIRenderer
    {
    public:

        ClIRenderer() {};

        ~ClIRenderer() {};
        
        virtual void SetIsResized(b8 bIsResized) = 0;

        virtual void Init() = 0;
        virtual void Destroy() = 0;

        virtual void Update() = 0;

        virtual void Render() = 0;

        // void ToggleFullScreen();

    protected:


    private:

    };
}