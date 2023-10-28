#pragma once

#include "ClVkRenderLayerBase.h"


namespace Clunk::Vk
{

    class ClVkEndLayer : public ClVkRenderLayerBase
    {
    public:

        ClVkEndLayer() : ClVkRenderLayerBase(nullptr) {}
        ClVkEndLayer(ClVkContext* pVkCtx, ClVkImage* pDepthImage);
        
        virtual ~ClVkEndLayer();

        virtual void DrawFrame(const VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

    protected:


    private:

    };
}