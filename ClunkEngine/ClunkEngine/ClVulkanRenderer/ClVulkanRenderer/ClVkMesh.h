#pragma once

#include <PCH/pch.h>
// #include <ClVkBuffer/ClVkArrayBuffer.h>
// #include <ClVkImage/ClVkTexture.h>
#include <ClMath/ClMath.h>
#include "ClVkVertex.h"
#include "ClVkMeshComp.h"


namespace Clunk
{
    class ClVkMesh
    {
    public:

        ClVkMesh() {};
        ClVkMesh(String FilePath, String TexturePath);
        ~ClVkMesh() {};

        void Load(String FilePath);
        void LoadTexture(String FilePath);
        void Unload();
        
        void Update(f32 DeltaTime);
        
        // void Render(const VkCommandBuffer& DrawBuffer, const VkPipelineLayout& PipelineLayout, const VkDescriptorSet* pDescriptorSet);
        void Render(const VkCommandBuffer& DrawBuffer, const VkPipelineLayout& PipelineLayout, const VkBuffer& TransformUBO);

        ClVkTexture* Texture() { return &m_Texture; }

        
    protected:

        Transform3d m_transform;
        // ClVkMeshComp m_meshComp;
        std::vector<Vertex3d> m_vertices;
        std::vector<u32> m_indices;

        ClVkArrayBuffer m_VertBuffer;
        ClVkArrayBuffer m_IndexBuffer;
        ClVkTexture m_Texture;

    private:


    };
}
