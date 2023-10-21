#pragma once

#include <vulkan/vulkan.h>
#include "ClVkRenderLayerBase.h"
#include <ClVulkan/VkShaderUtils.h>

namespace Clunk::Vk
{
    class ClVkSimple2dLayer : public ClVk2dLayer
    {
    public:

        ClVkSimple2dLayer(ClVkContext* pVkCtx);

        ~ClVkSimple2dLayer();

        virtual void Update(u32 CurrentIndex, f32 DeltaTime) override;

        virtual void DrawFrame(VkCommandBuffer& CmdBuffer, size_t CurrentImage) override;

    private:

        void CreatePipeline(std::vector<ClVkShaderModule> ShaderModules, ClVkRenderPass RenderPass, VkPipelineLayout Layout, VkExtent2D CustomExtent = VkExtent2D{ .width = 0, .height = 0 });

        void Draw(VkCommandBuffer CmdBuffer);
        
    };
}