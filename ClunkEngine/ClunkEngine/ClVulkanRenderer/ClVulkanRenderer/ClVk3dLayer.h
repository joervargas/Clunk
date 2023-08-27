#pragma once

#include "ClVkRenderLayerBase.h"
#include <ClVulkan/ClVkContext.h>

namespace Clunk::Vk
{
    class ClVk3dLayer : public ClVkRenderLayerBase
    {
    public:

        ClVk3dLayer(ClVkContext* pVkCtx, const char* ModelFile, const char* TextureFile, u32 UniformDataSize);

        ~ClVk3dLayer();


    private:

        void CreateDescriptorSet(ClVkContext& VkCtx, u32 UniformDataSize);

    };
}


