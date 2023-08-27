#include "ClVk3dLayer.h"


namespace Clunk::Vk
{
    ClVk3dLayer::ClVk3dLayer(ClVkContext* pVkCtx, const char* ModelFile, const char* TextureFile, u32 UniformDataSize) :
        ClVkRenderLayerBase(pVkCtx)
    {

    }

    ClVk3dLayer::~ClVk3dLayer()
    {

    }

    void ClVk3dLayer::CreateDescriptorSet(ClVkContext &VkCtx, u32 UniformDataSize)
    {

    }

}
