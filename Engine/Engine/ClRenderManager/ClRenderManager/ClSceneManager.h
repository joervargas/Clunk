#pragma once

#include <Core/Object.h>
#include <Core/ResourceManager.h>

#include "ClRenderManager.h"
#include "ClLayerStack.h"
#include <TinyXML2/tinyxml2.h>
#include <Defines/Defines.h>
#include <PCH/pch.h>


// typedef enum SceneEventType
// {
//     SE_TIMER_EXPIRED=0
// } SceneEventType;

// class clSceneManager;

// class clSceneListener : public clObject
// {

// public:

//     SceneEventType m_ListenFor;

//     virtual void Event(KSceneManager* SceneManager, void* CustomData) const = 0;
// };

namespace Clunk
{

    class clSceneObject : clRenderObject
    {

    };

    class clSceneManager : clResourceManager
    {
    public:

        clSceneManager() {};
        ~clSceneManager() {};

        bool Init();
        bool Update();
        void Destroy();

        void RenderScene();

        void ToggleFullScreen();

        clResource* LoadFromXML(tinyxml2::XMLElement* Element);
        clSceneObject* Add();

        void RenderAllObjects();

    protected:
    private:

        LayerStack m_layers;
    };
}
