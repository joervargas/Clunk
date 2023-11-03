#pragma once

#include "ClVkRenderLayerBase.h"


namespace Clunk::Vk
{

    class ClVkBeginLayer : public ClVkRenderLayerBase
    {
    public:

        ClVkBeginLayer() : ClVkRenderLayerBase() {};
        ClVkBeginLayer(const ClVkContext& VkCtx, ClVkImage* pDepthImage);

        ClVkBeginLayer(const ClVkBeginLayer& Other) : ClVkRenderLayerBase(Other) {}
        ClVkBeginLayer(ClVkBeginLayer&& Other) : ClVkRenderLayerBase(Other) {}

        ClVkBeginLayer& operator=(const ClVkBeginLayer& Other) 
        {
            ClVkRenderLayerBase::operator=(Other);
            return *this;
        }

        ClVkBeginLayer& operator=(ClVkBeginLayer&& Other)
        {
            ClVkRenderLayerBase::operator=(Other);
            return *this;
        }

        virtual ~ClVkBeginLayer();

        virtual void DrawFrame(const ClVkContext& VkCtx, const VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

    protected:

        // void BeginRenderPass(VkCommandBuffer CmdBuffer, u32 CurrentImage);
        // void EndRenderPass(VkCommandBuffer CmdBuffer);

    private:

    };

}