#pragma once

#include <vulkan/vulkan.h>
#include "ClVkRenderLayerBase.h"
#include <ClVulkan/VkShaderUtils.h>

#include <ClMath/Vec2.h>
#include <ClMath/Vec3.h>

#include <PCH/pch.h>

namespace Clunk::Vk
{
    struct Simple2dVertex
    {
        // glm::vec3 pos;
        // glm::vec3 color;
        // glm::vec2 texCoord;
        Clunk::Vec2 pos;
        Clunk::Vec3 color;
        Clunk::Vec2 texCoord;

        static VkVertexInputBindingDescription GetBindDesc()
        {
            VkVertexInputBindingDescription bindingDesc{};
            bindingDesc.binding = 0;
            bindingDesc.stride = sizeof(Simple2dVertex);
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
            attribDesc[0].offset = offsetof(Simple2dVertex, pos);
            // color
            attribDesc[1].binding = 0;
            attribDesc[1].location = 1;
            attribDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attribDesc[1].offset = offsetof(Simple2dVertex, color);
            // texCoord
            attribDesc[2].binding = 0;
            attribDesc[2].location = 2;
            attribDesc[2].format = VK_FORMAT_R32G32_SFLOAT,
            attribDesc[2].offset = offsetof(Simple2dVertex, texCoord);

            return attribDesc;
        }

        bool operator==(const Simple2dVertex& other) const
        {
            return pos == other.pos && color == other.color && texCoord == other.texCoord;
        }

        bool operator==(const Simple2dVertex& other)
        {
            return pos == other.pos && color == other.color && texCoord == other.texCoord;
        }
    };


    class ClVkSimple2dLayer : public ClVk2dLayer
    {
    public:

        ClVkSimple2dLayer(ClVkContext& VkCtx, const char* TextureFile = "");

        virtual ~ClVkSimple2dLayer();

        virtual void Destroy(ClVkContext& VkCtx) override;

        virtual void Update(ClVkContext& VkCtx, u32 CurrentIndex, f32 DeltaTime) override;

        virtual void DrawFrame(const ClVkContext& VkCtx, const VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

    protected:

        void CreateDescriptor(ClVkContext& VkCtx);

        void CreatePipeline(const ClVkContext& VkCtx, std::vector<ClVkShaderModule>& ShaderModules, ClVkRenderPass& RenderPass, VkPipelineLayout& Layout, VkExtent2D CustomExtent = VkExtent2D{ .width = 0, .height = 0 });

        void Draw(const ClVkContext& VkCtx, VkCommandBuffer CmdBuffer);
        
        ClVkBuffer mVerts;
        ClVkBuffer mIndices;

        ClVkImage mTexture;
        VkSampler mSampler;

        const std::vector<Simple2dVertex> VERTICES_DATA = {
            { Clunk::Vec2(-0.5f, -0.5f), Clunk::Vec3(1.0f, 0.0f, 0.0f), Clunk::Vec2(1.0f, 0.0f) },
            { Clunk::Vec2( 0.5f, -0.5f), Clunk::Vec3(0.0f, 1.0f, 0.0f), Clunk::Vec2(0.0f, 0.0f) },
            { Clunk::Vec2( 0.5f,  0.5f), Clunk::Vec3(0.0f, 0.0f, 1.0f), Clunk::Vec2(0.0f, 1.0f) },
            { Clunk::Vec2(-0.5f,  0.5f), Clunk::Vec3(1.0f, 1.0f, 1.0f), Clunk::Vec2(1.0f, 1.0f) }
        };

        const std::vector<u16> INDICE_DATA = {
            0, 1, 2, 2, 3, 0,
        };
    };

    // struct Simple2dVertex
    // {
    //     // glm::vec3 pos;
    //     // glm::vec3 color;
    //     // glm::vec2 texCoord;
    //     Clunk::Vec2 pos;
    //     Clunk::Vec3 color;
    //     Clunk::Vec2 texCoord;

    //     static VkVertexInputBindingDescription GetBindDesc()
    //     {
    //         VkVertexInputBindingDescription bindingDesc{};
    //         bindingDesc.binding = 0;
    //         bindingDesc.stride = sizeof(Simple2dVertex);
    //         bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    //         return bindingDesc;
    //     }

    //     static std::array<VkVertexInputAttributeDescription, 3> GetAttibDesc()
    //     {
    //         std::array<VkVertexInputAttributeDescription, 3> attribDesc{};
    //         // pos
    //         attribDesc[0].binding = 0;
    //         attribDesc[0].location = 0;
    //         attribDesc[0].format = VK_FORMAT_R32G32_SFLOAT,
    //         attribDesc[0].offset = offsetof(Simple2dVertex, pos);
    //         // color
    //         attribDesc[1].binding = 0;
    //         attribDesc[1].location = 1;
    //         attribDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    //         attribDesc[1].offset = offsetof(Simple2dVertex, color);
    //         // // texCoord
    //         attribDesc[2].binding = 0;
    //         attribDesc[2].location = 2;
    //         attribDesc[2].format = VK_FORMAT_R32G32_SFLOAT,
    //         attribDesc[2].offset = offsetof(Simple2dVertex, texCoord);

    //         return attribDesc;
    //     }

    //     bool operator==(const Simple2dVertex& other) const
    //     {
    //         // return pos == other.pos && color == other.color && texCoord == other.texCoord;
    //         return pos == other.pos && color == other.color;
    //     }

    //     bool operator==(const Simple2dVertex& other)
    //     {
    //         // return pos == other.pos && color == other.color && texCoord == other.texCoord;
    //         return pos == other.pos && color == other.color;
    //     }
    // };

}

namespace std
{
    template<> struct hash<Clunk::Vk::Simple2dVertex>
    {
        size_t operator()(const Clunk::Vk::Simple2dVertex& V) const
        {
            return ((hash<Clunk::Vec2>()(V.pos) ^
                    (hash<Clunk::Vec3>()(V.color) << 1)) >> 1) ^
                    (hash<Clunk::Vec2>()(V.texCoord) << 1);
        }
    };
}

// const std::vector<Clunk::Vk::Simple2dVertex> VERTICES_DATA = {
//     { Clunk::Vec2(-0.5f, -0.5f), Clunk::Vec3(1.0f, 0.0f, 0.0f), Clunk::Vec2(1.0f, 0.0f) },
//     { Clunk::Vec2( 0.5f, -0.5f), Clunk::Vec3(0.0f, 1.0f, 0.0f), Clunk::Vec2(0.0f, 0.0f) },
//     { Clunk::Vec2( 0.5f,  0.5f), Clunk::Vec3(0.0f, 0.0f, 1.0f), Clunk::Vec2(0.0f, 1.0f) },
//     { Clunk::Vec2(-0.5f,  0.5f), Clunk::Vec3(1.0f, 1.0f, 1.0f), Clunk::Vec2(1.0f, 1.0f) }
// };

// const std::vector<u16> INDICE_DATA = {
//     0, 1, 2, 2, 3, 0,
// };