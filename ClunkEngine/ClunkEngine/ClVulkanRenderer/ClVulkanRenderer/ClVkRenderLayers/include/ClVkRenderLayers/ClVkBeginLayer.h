#pragma once

#include "ClVkRenderLayerBase.h"


namespace Clunk::Vk
{

    class ClVkBeginLayer : public ClVkRenderLayerBase
    {
    public:

        ClVkBeginLayer() : ClVkRenderLayerBase(nullptr) {};
        ClVkBeginLayer(ClVkContext* pVkCtx, ClVkImage* pDepthImage);
        virtual ~ClVkBeginLayer();

        virtual void DrawFrame(const VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

    protected:

        // void BeginRenderPass(VkCommandBuffer CmdBuffer, u32 CurrentImage);
        // void EndRenderPass(VkCommandBuffer CmdBuffer);

    private:

    };

}