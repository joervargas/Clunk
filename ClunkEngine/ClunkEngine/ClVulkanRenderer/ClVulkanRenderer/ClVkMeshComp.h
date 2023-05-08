#pragma once

#include <ClRenderManager/ClIMeshComp.h>
#include <PCH/pch.h>

#include <ClVkBuffer/ClVkArrayBuffer.h>
#include <ClVkImage/ClVkTexture.h>
#include <ClMath/ClMath.h>
#include "ClVkVertex.h"



namespace Clunk
{
    class ClVkMeshComp : public ClIMeshComp
    {
    public:

        // TODO: Make VkPipelineLayout a member variable
        
        virtual void Load() {};
        virtual void LoadFromFile(String MeshPath, String TexturePath = "", Vec3 Color = Vec3(1.f, 0.6f, 0.f));
        virtual void Unload();

        virtual void Render(
            const VkCommandBuffer& DrawBuffer,
            const VkPipelineLayout& PipelineLayout,
            const VkBuffer& TransformUBO,
            const Transform3d& MeshTransform
        );

        virtual void Destroy();

    protected:

        ClVkArrayBuffer m_VertBuffer;
        ClVkArrayBuffer m_IndexBuffer;
        ClVkTexture m_Texture;
        Vec3 m_color;

    private:


    };
}