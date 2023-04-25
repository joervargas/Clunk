#pragma once

#include <ClRenderManager/ClIRenderer.h>


#include <ClVkContext/ClVkContext.h>
#include <ClVkSwapchain/ClVkSwapchain.h>
#include <ClVkPipeline/ClVkGraphicsPipeline.h>
#include <ClVkPipeline/ClVkPipelineLayout.h>
#include <ClVkPipeline/ClVkRenderPass.h>
#include <ClVkCmds/ClVkCmdPools.h>
#include <ClVkCmds/ClVkCmds.h>

#include <ClVkBuffer/ClVkArrayBuffer.h>
#include <ClVkBuffer/ClVkUniformBuffer.h>
#include "ClVkVertex.h"
#include "ClVkTransforms.h"

#include <ClVkImage/ClVkDepthImage.h>
#include <ClVkImage/ClVkTexture.h>

#include <ClVkDescriptor/ClVkDescriptorSetLayout.h>
#include <ClVkDescriptor/ClVkDescriptorPool.h>
#include <ClVkDescriptor/ClVkDescriptorSet.h>
#include "ClVkMesh.h"

#include <Defines/Defines.h>
#include <ClPlatforms/ClPlatform.h>

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#include <ClMath/ClMath.h>

namespace Clunk
{
    class ClVkRenderer : public ClIRenderer
    {
    public:

        virtual void SetIsResized(b8 bIsResized) override { m_bIsResized = bIsResized; }
        
        virtual void Init() override;
        virtual void Update() override;
        virtual void Destroy() override;

        virtual void ResetFrame();

        virtual void RenderFrame() override;

        void RecordDrawCmdBuffer(const VkCommandBuffer& DrawBuffer, const u32& ImageIndex);

    private:

        ClVkContext* m_vkCtx;
        ClVkSwapchain* m_swapchain;
        b8 m_bIsResized{false};

        // ClVkDepthImage m_depthImage;

        ClVKRenderPass m_renderPass;
        ClVkPipelineLayout m_pipelineLayout;
        ClVkGraphicsPipeline m_basicPipeline;

        ClVkDescriptorSetLayout m_descSetLayout;
        ClVkDescriptorPool m_descPool;
        ClVkDescriptorSets m_descSets;

        ClVkCmdPools* m_cmdPools;
        std::vector<VkCommandBuffer> m_drawCmdBuffers;

        const u32 MAX_FRAMES_IN_FLIGHT{2};
        u32 m_currentFrame{0};
        f32 m_lastTime{0.0f};
        
        // ClVkArrayBuffer m_VAO;
        // ClVkArrayBuffer m_IBO;
        // ClVkTexture m_Texture;
        ClVkMesh m_Mesh;

        struct ClVkTransforms m_worldTransform;

        ClVkUniformBuffers<ClVkTransforms> m_worldTransformUBOs;


    };
}