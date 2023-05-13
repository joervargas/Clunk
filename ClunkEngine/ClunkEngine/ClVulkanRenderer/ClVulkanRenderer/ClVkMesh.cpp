#include "ClVkMesh.h"

#include <TinyObjLoader/tiny_obj_loader.h>
// #include <MeshOptimizer/meshoptimizer.h>

#include <Core/Exception.h>
#include <Core/Logger.h>
#include <ClVkContext/ClVkContext.h>
#include <ClVkCmds/ClVkCmdPools.h>
#include "ClVkTransforms.h"
#include <unordered_map>

#include <vulkan/vulkan.h>
#include <ClVkExt/ClVkExt.h>

namespace Clunk
{
    ClVkMesh::ClVkMesh(String FilePath, String TexturePath)
    {
        Load(FilePath);
        LoadTexture(TexturePath);
        // m_meshComp.LoadFromFile(FilePath, TexturePath);
        m_transform = Transform3d(Vec3(0.0f, 0.0f, 0.1f));
    }

    void ClVkMesh::Load(String FilePath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        String warn, err;

        if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, FilePath.c_str()))
        {
            String err_string = warn + err;
            THROW_EXCEPTION("Failed to load Mesh!: %s, %s", FilePath.c_str(), err_string.c_str());
        }
        
        std::unordered_map<Vertex3d, u32> uniqueVertices{};
        for (const auto& shape : shapes)
        {
            for(const auto& index : shape.mesh.indices)
            {
                Vertex3d vertex{};
                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
                vertex.color = {1.0f, 1.0f, 1.0f};

                if(uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<u32>(m_vertices.size());
                    m_vertices.push_back(vertex);
                }
                m_indices.push_back(uniqueVertices[vertex]);
            } // shape.mesh.indices
        } // shapes

        ClVkCmdPools* cmd_pools = ClVkCmdPools::Get();
        ClVkContext* vkctx = ClVkContext::Get();
        m_VertBuffer.Create<Vertex3d>(*cmd_pools->GetGraphicsPool(), *vkctx->QueueFamilies->Graphics(), m_vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        m_IndexBuffer.Create<u32>(*cmd_pools->GetGraphicsPool(), *vkctx->QueueFamilies->Graphics(), m_indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        // m_position = Vec3(0.0f, 0.0f, 0.1f);
        m_transform = Transform3d(Vec3(0.0f, 0.0f, 0.1f));
    }

    void ClVkMesh::LoadTexture(String FilePath)
    {
        m_Texture.FromFile(FilePath.c_str());
    }

    void ClVkMesh::Unload()
    {
        m_Texture.Destroy();
        m_VertBuffer.Destroy();
        m_IndexBuffer.Destroy();

        m_vertices.clear();
        m_indices.clear();

        // m_meshComp.Unload();
    }

    void ClVkMesh::Update(f32 DeltaTime)
    {
        m_transform.rotation.SetAngleAxis(Math::DegToRad(30.f) * DeltaTime, Vec3(0.0f, 0.0f, 1.0f));
    }

    // void ClVkMesh::Render(const VkCommandBuffer& DrawBuffer, const VkPipelineLayout& PipelineLayout, const VkDescriptorSet* pDescriptorSet)
    void ClVkMesh::Render(const VkCommandBuffer& DrawBuffer, const VkPipelineLayout& PipelineLayout, const VkBuffer& TransformUBO)
    {
        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(DrawBuffer, 0, 1, &m_VertBuffer.buffer, &offset);
        vkCmdBindIndexBuffer(DrawBuffer, m_IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
        
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = TransformUBO;
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(ClVkTransforms);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = *m_Texture.GetView();
        imageInfo.sampler = *m_Texture.GetSampler();
        
        std::array<VkWriteDescriptorSet, 2> descWrites{};
        descWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrites[0].dstSet = 0;
        descWrites[0].dstBinding = 0;
        descWrites[0].dstArrayElement = 0;
        descWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descWrites[0].descriptorCount = 1;
        descWrites[0].pBufferInfo = &bufferInfo;
        descWrites[0].pImageInfo = nullptr;
        descWrites[0].pTexelBufferView = nullptr;

        descWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrites[1].dstSet = 0;
        descWrites[1].dstBinding = 1;
        descWrites[1].dstArrayElement = 0;
        descWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descWrites[1].descriptorCount = 1;
        descWrites[1].pBufferInfo = nullptr;
        descWrites[1].pImageInfo = &imageInfo;
        descWrites[1].pTexelBufferView = nullptr;
        vk_ext::vkCmdPushDescriptorSetKHR(DrawBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PipelineLayout, 0, 2, descWrites.data());

        vkCmdPushConstants(DrawBuffer, PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Transform3d), &m_transform);
        // vkCmdBindDescriptorSets(DrawBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PipelineLayout, 0, 1, pDescriptorSet, 0, nullptr);

        vkCmdDrawIndexed(DrawBuffer, m_IndexBuffer.len, 1, 0, 0, 0);

        // m_meshComp.Render(DrawBuffer, PipelineLayout, TransformUBO, m_transform);
    }

} // namespace Clunk
