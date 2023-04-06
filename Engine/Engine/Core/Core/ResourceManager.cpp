#include "ResourceManager.h"

#include "Logger.h"
// #include "LogManager.h"

#include <TinyXML2/tinyxml2.h>


namespace Clunk
{
    b8 clResourceManager::LoadFromXMLFile(String Filename)
    {

        using namespace tinyxml2;

        XMLDocument doc;
        XMLError err = doc.LoadFile(Filename.c_str()); // Load the XML file
        if(err != XMLError::XML_SUCCESS) // if Loading file was unsuccessful
        {
            CLOG_ERROR("Resource Manager unable to load resource XML file: %s", Filename.c_str());
            // THROW_EXCEPTION("Error: Resource Manager failed to to load XML file: %s", Filename.c_str());
            return false;
        }

        XMLElement* ResourceTree = doc.FirstChildElement("resources"); // Get the first resource element in the file
        if(ResourceTree)
        {
            for(XMLNode* child = ResourceTree->FirstChild(); child; child = child->NextSibling()) // Scan the tree
            {
                XMLElement *element = child->ToElement();
                if(element)
                {
                    clResource *resource = nullptr;
                    for(const XMLAttribute *elementAttrib = element->FirstAttribute(); elementAttrib; elementAttrib = elementAttrib->Next())
                    {
                        // Examine the resource
                        String attribName = elementAttrib->Name();
                        String attribValue = elementAttrib->Value();

                        if(attribName == "type")
                        {
                            /**
                             * Allow managers to implement their own derived versions of Resource class.
                             * Appropriate manager creates resources and passes back a Resource* to be added to the
                             * resource list.
                             */
                            if(attribValue == "graphic")
                            {
                                // resource = RenderManager->loadResourceFromXML(element);
                            }
                            if(attribValue == "audio")
                            {
                                // resource = AudioManager->loadResourceFromXML(element);
                            }
                            if(attribValue == "text")
                            {
                                // resource = ConfigManager->loadResourceFromXML(element);
                            }
                            if(resource)
                            {
                                if(attribName == "UID")
                                {
                                    resource->SetResourceID( atoi(attribValue.c_str()) );
                                }
                                if(attribName == "filename")
                                {
                                    resource->SetFileName(attribValue);
                                }
                                if(attribName == "scenescope")
                                {
                                    resource->SetScope( atoi(attribValue.c_str()) );
                                }

                            }
                        }
                        if(resource)
                        {
                            resources[resource->GetScope()].push_back(resource);
                            m_ResourceCount++;
                        }
                    }
                }
            }
            return true;
        }
        return false;
    }

    clResource* clResourceManager::FindByID(u32 ResourceID)
    {
        std::map<u32, std::list<clResource*>>::iterator iter; // Map iterator

        for(iter = resources.begin(); iter != resources.end(); iter++) // for each scope
        {
            if(!iter->second.empty()) // if scope's list is not empty
            {
                std::list<clResource*>::iterator list_iter;
                for(list_iter = iter->second.begin(); list_iter != iter->second.end();  list_iter++) // for each Resource* in scope
                {
                    // If Resource matches given ResourceID
                    if((*list_iter)->GetResourceID() == ResourceID) return (*list_iter);
                }
            }
        }
        return nullptr;
    }

    void clResourceManager::Clear()
    {
        std::map<u32, std::list<clResource*>>::iterator iter;

        for(iter = resources.begin(); iter != resources.end(); iter++) // for each scope
        {
            if(!iter->second.empty()) // if scope is not empty
            {
                std::list<clResource*>::iterator list_iter;
                for(list_iter = iter->second.begin(); list_iter != iter->second.end(); list_iter++) // for each Resource* in scope
                {
                    (*list_iter)->unload();
                    SDELETE(*list_iter);
                }
                iter->second.clear();
            }
        }
        resources.clear();
    }

    void clResourceManager::SetCurrentScope(const u32 &CurrentScope)
    {
        // Unload old scope if not global scope
        if(m_CurrentScope != 0)
        {
            std::list<clResource*>::iterator list_iter;

            for(list_iter = resources[m_CurrentScope].begin(); list_iter != resources[m_CurrentScope].end(); list_iter++)
            {
                (*list_iter)->unload();
            }
        }

        m_CurrentScope = CurrentScope;

        // Load new scope
        std::list<clResource*>::iterator list_iter;
        for(list_iter = resources[m_CurrentScope].begin(); list_iter != resources[m_CurrentScope].end(); list_iter++)
        {
            (*list_iter)->load();
        }
    }

}
