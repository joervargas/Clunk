#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Object.h"
#include "Resource.h"

#include <PCH/pch.h>

#include <list>
#include <map>



namespace Clunk
{

    class clResourceManager : public ClObject
    {
    public:

        clResourceManager()
        {
            m_CurrentScope = m_ResourceCount = 0;
        }

        std::map<u32, std::list<clResource*>> resources; // mapped linked list of Resource* where u32 = Resource Scope, 

        /**
         * @brief Loads resources from XML File
         * @param Filename Path to the XML File
         * @return b8 True on success, False on failure
         */
        b8 LoadFromXMLFile(String Filename);

        /**
         * @brief Finds an Resource* from the given ID. Returns nullptr if not found.
         * @param ResourceID The unique identifier of the Resource.
         * @return Resource* 
         */
        clResource* FindByID(u32 ResourceID);

        /**
         * @brief Clears all resources and scopes.
         */
        void Clear();

        u32 GetCurrentScope() const { return m_CurrentScope; }
        void SetCurrentScope(const u32 &CurrentScope);

        u32 GetResourceCount() const { return m_ResourceCount; }

    private: 

        u32 m_CurrentScope; // Current resource scope
        u32 m_ResourceCount; // Total number of resources in existance both loaded and unloaded

    };

}

#endif