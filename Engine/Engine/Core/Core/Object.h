#ifndef EOBJECT_H
#define EOBJECT_H

#include <Defines/Defines.h>

namespace Clunk
{
    
    class ClObject
    {
    public:

        u32 GetObjectID() const { return m_ID; }

        ClObject() { m_ID = OBJECT_COUNT++; }
        ~ClObject() {};

    protected:
    private:

        inline static u32 OBJECT_COUNT;
        u32 m_ID;
    };

}

#endif