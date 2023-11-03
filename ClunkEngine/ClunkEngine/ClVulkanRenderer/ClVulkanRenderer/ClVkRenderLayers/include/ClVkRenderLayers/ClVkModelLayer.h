#pragma once

#include "ClVkRenderLayerBase.h"
#include <ClVulkan/ClVkContext.h>
#include <ClVulkan/ClVkImg.h>
#include <ClVulkan/ClVkRenderPass.h>

#include <PCH/pch.h>

namespace Clunk::Vk
{

    class ClVkModelLayer : public ClVkRenderLayerBase
    {
    public:

        ClVkModelLayer(const ClVkContext& VkCtx, const char* ModelFile, const char* TextureFile, u32 UniformDataSize);

        virtual void DrawFrame(const ClVkContext& VkCtx, const VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

    private:

        size_t mVertexBufferSize = 0;
        size_t mIndexBufferSize = 0;

        VkBuffer mStorageBuffer;
        VkDeviceMemory mStorageBufferMemory;

        VkSampler mTextureSampler;
        ClVkImage mTexture;

        void CreatePipeline();
    };

}