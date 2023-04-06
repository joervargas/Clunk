#ifndef RESOURCE_H
#define RESOURCE_H

#include "Object.h"

#include <Defines/Defines.h>
#include <PCH/pch.h>


namespace Clunk
{

    /// @brief Indicates the type category of the resource
    enum EResourceType: u8
    {
        R_NULL,
        R_GRAPHIC,
        R_MOVIE,
        R_AUDIO,
        R_TEXT,
    };

    class clResource : public ClObject
    {
    public:

        clResource()
        {
            // m_ResourceID = TOTAL_RESOURCE_COUNT++;
            m_Scope = 0;
            m_Type = R_NULL;
        }

        virtual ~clResource() {};
        virtual void load() {};
        virtual void unload() {};

        u32 GetResourceID() const { return m_ResourceID; }
        void SetResourceID(const u32 &ResourceID) { m_ResourceID = ResourceID; }

        u32 GetScope() const { return m_Scope; }
        void SetScope(const u32 &Scope) { m_Scope = Scope; }

        String GetFileName() const { return m_FileName; }
        void SetFileName(const String &FileName) { m_FileName = FileName; }

        EResourceType GetType() const { return m_Type; }
        void SetType(const EResourceType &Type) { m_Type = Type; }

    protected:

        
    private:

        // inline static u32 TOTAL_RESOURCE_COUNT; // Resource Count to be incremented. Used to get a uniqe identifier.

        u32 m_ResourceID; // Unique identifier. Taken from incrementing static TotalResourceCount value.
        u32 m_Scope; // Determines the scope of the asset. Game wide scope or to a scene.
        String m_FileName; // Full relative path to the resource file.
        EResourceType m_Type; // The type category of the current resource.

    };

}

#endif