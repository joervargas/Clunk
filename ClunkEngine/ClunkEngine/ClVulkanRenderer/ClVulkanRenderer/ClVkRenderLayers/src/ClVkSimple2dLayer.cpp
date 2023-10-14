#include "ClVkRenderLayers/ClVkSimple2dLayer.h"
#include "ClVkSimple2dLayer.h"

namespace Clunk::Vk
{
    ClVkSimple2dLayer::ClVkSimple2dLayer(ClVkContext *pVkCtx) :
        ClVk2dLayer(pVkCtx)
    {

    }

    ClVkSimple2dLayer::~ClVkSimple2dLayer()
    {
    }

    void ClVkSimple2dLayer::DrawFrame(VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {
        BeginRenderPass(CmdBuffer, CurrentImage);
        Draw(CmdBuffer);
        EndRenderPass(CmdBuffer);
    }

    void ClVkSimple2dLayer::Draw(VkCommandBuffer CmdBuffer)
    {
    }
}