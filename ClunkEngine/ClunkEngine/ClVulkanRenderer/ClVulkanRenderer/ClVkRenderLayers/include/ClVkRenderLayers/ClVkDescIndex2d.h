#pragma once

#include <vulkan/vulkan.h>
#include "ClVkRenderLayerBase.h"
#include <ClVulkan/VkShaderUtils.h>

#include <ClMath/Vec2.h>
#include <ClMath/Vec3.h>

#include <PCH/pch.h>
#include <array>

namespace Clunk::Vk
{
    struct DescIndex2dVertex
    {
        // glm::vec3 pos;
        // glm::vec2 texCoord;
        Clunk::Vec2 pos;
        Clunk::Vec2 texCoord;

        static VkVertexInputBindingDescription GetBindDesc()
        {
            VkVertexInputBindingDescription bindingDesc{};
            bindingDesc.binding = 0;
            bindingDesc.stride = sizeof(DescIndex2dVertex);
            bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDesc;
        }

        static std::array<VkVertexInputAttributeDescription, 3> GetAttibDesc()
        {
            std::array<VkVertexInputAttributeDescription, 3> attribDesc{};
            // pos
            attribDesc[0].binding = 0;
            attribDesc[0].location = 0;
            attribDesc[0].format = VK_FORMAT_R32G32_SFLOAT,
            attribDesc[0].offset = offsetof(DescIndex2dVertex, pos);
            // texCoord
            attribDesc[2].binding = 0;
            attribDesc[2].location = 1;
            attribDesc[2].format = VK_FORMAT_R32G32_SFLOAT,
            attribDesc[2].offset = offsetof(DescIndex2dVertex, texCoord);

            return attribDesc;
        }

        bool operator==(const DescIndex2dVertex& other) const
        {
            return pos == other.pos && texCoord == other.texCoord;
        }

        bool operator==(const DescIndex2dVertex& other)
        {
            return pos == other.pos && texCoord == other.texCoord;
        }
    };


    class ClVkDescIndex2dLayer : public ClVk2dLayer
    {
    public:

        ClVkDescIndex2dLayer(ClVkContext& VkCtx);

        virtual ~ClVkDescIndex2dLayer();

        virtual void Destroy(ClVkContext& VkCtx) override;

        virtual void Update(u32 CurrentIndex, f32 DeltaTime) override;

        virtual void DrawFrame(const ClVkContext& VkCtx, const VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

    private:

        void CreateDescriptor(ClVkContext& VkCtx);

        void CreatePipeline(const ClVkContext& VkCtx, std::vector<ClVkShaderModule>& ShaderModules, ClVkRenderPass& RenderPass, VkPipelineLayout& Layout, VkExtent2D CustomExtent = VkExtent2D{ .width = 0, .height = 0 });

        void Draw(const ClVkContext& VkCtx, VkCommandBuffer CmdBuffer);
        
        ClVkBuffer mVerts;
        ClVkBuffer mIndices;

        std::vector<ClVkImage> mTextures;
        std::vector<VkSampler> mSamplers;

        const std::vector<DescIndex2dVertex> VERTICES_DATA = {
            { Clunk::Vec2(-0.5f, -0.5f), Clunk::Vec2(1.0f, 0.0f) },
            { Clunk::Vec2( 0.5f, -0.5f), Clunk::Vec2(0.0f, 0.0f) },
            { Clunk::Vec2( 0.5f,  0.5f), Clunk::Vec2(0.0f, 1.0f) },
            { Clunk::Vec2(-0.5f,  0.5f), Clunk::Vec2(1.0f, 1.0f) }
        };

        const std::vector<u16> INDICE_DATA = {
            0, 1, 2, 2, 3, 0,
        };
    };
}

namespace std
{
    template<> struct hash<Clunk::Vk::DescIndex2dVertex>
    {
        size_t operator()(const Clunk::Vk::DescIndex2dVertex& V) const
        {
            return ((hash<Clunk::Vec2>()(V.pos) ^
                    (hash<Clunk::Vec2>()(V.texCoord) << 1)) >> 1);
        }
    };
}