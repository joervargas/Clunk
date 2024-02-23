#pragma once

#include "ClVkRenderLayerBase.h"
#include <ClVulkan/VkShaderUtils.h>

#include <ClMath/ClMath.h>

#include <PCH/pch.h>


namespace Clunk::Vk
{
    struct Simple3dVertex
    {
        Clunk::Vec3 pos;
        Clunk::Vec3 color;
        Clunk::Vec2 texCoord;

        static VkVertexInputBindingDescription GetBindDesc()
        {
            VkVertexInputBindingDescription bindingDesc{};
            bindingDesc.binding = 0;
            bindingDesc.stride = sizeof(Simple3dVertex);
            bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDesc;
        }

        static std::array<VkVertexInputAttributeDescription, 3> GetAttibDesc()
        {
            std::array<VkVertexInputAttributeDescription, 3> attribDesc{};
            // pos
            attribDesc[0].binding = 0;
            attribDesc[0].location = 0;
            attribDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT,
            attribDesc[0].offset = offsetof(Simple3dVertex, pos);
            // color
            attribDesc[1].binding = 0;
            attribDesc[1].location = 1;
            attribDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attribDesc[1].offset = offsetof(Simple3dVertex, color);
            // texCoord
            attribDesc[2].binding = 0;
            attribDesc[2].location = 2;
            attribDesc[2].format = VK_FORMAT_R32G32_SFLOAT,
            attribDesc[2].offset = offsetof(Simple3dVertex, texCoord);

            return attribDesc;
        }

        bool operator==(const Simple3dVertex& other) const
        {
            return pos == other.pos && color == other.color && texCoord == other.texCoord;
        }

        bool operator==(const Simple3dVertex& other)
        {
            return pos == other.pos && color == other.color && texCoord == other.texCoord;
        }
    };

    class ClVkSimple3dLayer : public ClVk3dLayer
    {
    public:

        ClVkSimple3dLayer(ClVkContext &VkCtx, const ClVkImage& DepthImage, const ClVkBuffer& TransformUniform, const char* MeshFile, const char *TextureFile);

        virtual ~ClVkSimple3dLayer();

        virtual void Destroy(ClVkContext& VkCtx) override;

        virtual void Update(ClVkContext& VkCtx, u32 CurrentIndex, ClVkBuffer& TransformUniform, const ClVkTransforms& Transforms, f32 DeltaTime) override;

        virtual void DrawFrame(const ClVkContext& VkCtx, const VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

    protected:

        ClVkBuffer mVerts;
        ClVkBuffer mIndices;

        ClVkBuffer mModelSpaceBuffer;
        Mat4 mModelSpace;
        Transform3d mModelTransforms;

        ClVkImage mTexture;
        VkSampler mSampler;

    protected:

        void CreateDescriptor(ClVkContext& VkCtx, const ClVkBuffer& TransformUniform);

        void CreatePipeline(const ClVkContext& VkCtx, std::vector<ClVkShaderModule>& ShaderModules, ClVkRenderPass& RenderPass, VkPipelineLayout& Layout, VkExtent2D CustomExtent = VkExtent2D{ .width = 0, .height = 0 });

        void Draw(const ClVkContext& VkCtx, VkCommandBuffer CmdBuffer);

    };
}

namespace std
{
    template<> struct hash<Clunk::Vk::Simple3dVertex>
    {
        size_t operator()(const Clunk::Vk::Simple3dVertex& V) const
        {
            return ((hash<Clunk::Vec3>()(V.pos) ^
                    (hash<Clunk::Vec3>()(V.color) << 1)) >> 1) ^
                    (hash<Clunk::Vec2>()(V.texCoord) << 1);
        }
    };
}