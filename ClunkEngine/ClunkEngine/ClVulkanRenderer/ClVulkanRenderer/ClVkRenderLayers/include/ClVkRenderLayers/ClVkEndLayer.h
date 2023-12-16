#pragma once

#include "ClVkRenderLayerBase.h"


namespace Clunk::Vk
{

    class ClVkEndLayer : public ClVkRenderLayerBase
    {
    public:

        ClVkEndLayer() {}
        ClVkEndLayer(const ClVkContext& VkCtx, ClVkImage* pDepthImage);
        
        virtual ~ClVkEndLayer();

        virtual void DrawFrame(const ClVkContext& VkCtx, const VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

    protected:


    private:

    };
}