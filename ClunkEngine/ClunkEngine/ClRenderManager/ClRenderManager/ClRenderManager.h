#pragma once

#include <TinyXML2/tinyxml2.h>
#include <SDL.h>

#include <PCH/pch.h>
#include <sstream>

#include <Core/ResourceManager.h>
#include <Core/Object.h>

namespace Clunk
{

    /**
     * @brief Derived from Resource: Represents a graphics resource.
     * Pointers to instances of this class are maintained by the ResourceManager.
     */
    class ClRenderResource : public clResource
    {
    public:

        ClRenderResource();
        ~ClRenderResource();

        void Load();
        void Unload();

    protected:

    private:

    };

    /**
     * @brief clRenderObject. Each instance represents a renderable item in a 3D scene.
     * It is a reference to a RenderResource2D with position, scale, and roatation values.
     */
    class clRenderObject : ClObject
    {
    public:


        clRenderObject();
        ~clRenderObject();

        virtual void SetRenderResource(ClRenderResource *RenderResource);

        virtual void Load();
        virtual void Unload();
        virtual void Update(){};
        virtual void Draw(){};
        virtual void Destroy(){};

    protected:


    private:

        ClRenderResource *m_RenderResource;
        b8 m_bVisible;

    };


    // class ClRenderManager
    // {
    // public:

    //     b8 bIsResized{false};
    //     static ClRenderManager* GetRenderManager();
        
    //     bool Init();
    //     bool Update();
    //     void Destroy();

    //     // void RecreateSwapchain();
    //     // void DestroySwapchain();

    //     void RenderFrame();

    //     void ToggleFullScreen();

    //     // clResource* LoadResourceFromXML(tinyxml2::XMLElement* Element);
    //     // void RenderAllObjects();

    // protected:


    // private:

    //     static ClRenderManager* m_RenderManager;
    //     std::list<clRenderObject*> m_RenderObjects;
    //     // TODO: pointer to scene manager
    // };

}
