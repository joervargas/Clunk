#pragma once

#include <Defines/Defines.h>


namespace Clunk
{
    class ClIMeshComp
    {
    public:

        virtual void Load() = 0;
        virtual void Unload() = 0;
        // virtual void Render() {};
        virtual void Destroy() = 0;

    protected:


    private:

        b8 m_bVisible;

    };
}