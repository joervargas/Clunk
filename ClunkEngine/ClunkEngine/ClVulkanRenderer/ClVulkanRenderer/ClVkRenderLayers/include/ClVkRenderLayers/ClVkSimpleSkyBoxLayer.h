#pragma once

#include "ClVkRenderLayerBase.h"
#include "ClVulkan/VkShaderUtils.h"
#include <PCH/pch.h>
#include <ClMath/ClMath.h>

namespace Clunk::Vk
{
    struct SimpleSkyBoxVertex
    {
        Clunk::Vec3 pos;

        static VkVertexInputBindingDescription GetBindDesc()
        {
            VkVertexInputBindingDescription bindingDesc{};
            bindingDesc.binding = 0;
            bindingDesc.stride = sizeof(SimpleSkyBoxVertex);
            bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDesc;
        }

        static std::array<VkVertexInputAttributeDescription, 1> GetAttibDesc()
        {
            std::array<VkVertexInputAttributeDescription, 1> attribDesc{};
            // pos
            attribDesc[0].binding = 0;
            attribDesc[0].location = 0;
            attribDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT,
            attribDesc[0].offset = offsetof(SimpleSkyBoxVertex, pos);

            return attribDesc;
        }

        bool operator==(const SimpleSkyBoxVertex& other) const
        {
            return pos == other.pos;
        }

        bool operator==(const SimpleSkyBoxVertex& other)
        {
            return pos == other.pos;
        }
    };

    class ClVkSimpleSkyBoxLayer : public ClVk3dLayer
    {
    public:

        ClVkSimpleSkyBoxLayer(ClVkContext &VkCtx, const ClVkImage& DepthImage, const ClVkBuffer& TransformUniform, std::vector<const char*> TextureFiles);

        virtual ~ClVkSimpleSkyBoxLayer();

        virtual void Destroy(ClVkContext& VkCtx) override;

        virtual void Update(ClVkContext& VkCtx, u32 CurrentIndex, ClVkBuffer& ProjViewUniform, const ClProjectionView& ProjView, f32 DeltaTime) override;

        virtual void DrawFrame(const ClVkContext& VkCtx, const VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

    protected:

        void CreateDescriptor(ClVkContext& VkCtx, const ClVkBuffer& TransformUniform);

        void CreatePipeline(const ClVkContext& VkCtx, std::vector<ClVkShaderModule>& ShaderModules, ClVkRenderPass& RenderPass, VkPipelineLayout& Layout, VkExtent2D CustomExtent = VkExtent2D{ .width = 0, .height = 0 });

        void Draw(const ClVkContext& VkCtx, VkCommandBuffer CmdBuffer);

        ClVkBuffer mVerts;
        ClVkBuffer mIndices;

        ClVkBuffer mModelSpaceBuffer;
        Mat4 mModelSpace;

        ClVkImage mTexture;
        VkSampler mSampler;
    };
}

namespace std
{
    template<> struct hash<Clunk::Vk::SimpleSkyBoxVertex>
    {
        size_t operator()(const Clunk::Vk::SimpleSkyBoxVertex& V) const
        {
            return ((hash<f32>()(V.pos.X) ^
                    (hash<f32>()(V.pos.Y) << 1)) >> 1) ^
                    (hash<f32>()(V.pos.Z) << 1);
        }
    };
}