#pragma once

#include <Defines/Defines.h>

namespace Clunk
{
    class clLayer
    {
    public:

        clLayer();
        virtual ~clLayer() = default;

        virtual void OnAttach() {};
        virtual void OnDetach() {};
        virtual void OnUpdate() {};
        virtual void OnEvent() {};

    protected:

    private:

        
    };
}