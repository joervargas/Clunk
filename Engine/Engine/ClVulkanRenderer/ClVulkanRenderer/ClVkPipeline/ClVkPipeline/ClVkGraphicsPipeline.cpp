#include "ClVkGraphicsPipeline.h"
#include "ClVkShaderLoader.h"

#include <ClVkContext/ClVkDevice.h>
#include <ClVkSwapchain/ClVkSwapchain.h>

#include <Core/Logger.h>
#include <Core/Exception.h>

#include "ClVkVertex.h"
// #include <unistd.h>
// #include <filesystem>

namespace Clunk
{

    // void ClVkGraphicsPipeline::Create(const VkRenderPass& RenderPass, const VkSampleCountFlagBits& MsaaSampleCount, const VkDescriptorSetLayout* DescSetLayout)
    void ClVkGraphicsPipeline::Create(const VkRenderPass& RenderPass, const VkSampleCountFlagBits& MsaaSampleCount, const VkPipelineLayout* pPipelineLayout)
    {
        // CLOG_WARN("The current directory is %s", get_current_dir_name());
        // CLOG_WARN("The current directory is %s", std::filesystem::current_path().c_str() );
        CLOG_INFO("Creating VkPipelines...");

        ClVkShaderLoader depthShader;
        depthShader.SetStage("../Shaders/SPIRV/PushConst.vert.spirv", VK_SHADER_STAGE_VERTEX_BIT);
        depthShader.SetStage("../Shaders/SPIRV/PushConst.frag.spirv", VK_SHADER_STAGE_FRAGMENT_BIT);
        depthShader.PopulateShaderStages();

        auto vertBindDesc = Vertex3d::GetBindDesc();
        auto vertAttribDesc = Vertex3d::GetAttibDesc();

        VkPipelineVertexInputStateCreateInfo vertexInputCI{};
        vertexInputCI.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputCI.vertexBindingDescriptionCount = 1;
        vertexInputCI.pVertexBindingDescriptions = &vertBindDesc;
        vertexInputCI.vertexAttributeDescriptionCount = static_cast<u32>(vertAttribDesc.size());
        vertexInputCI.pVertexAttributeDescriptions = vertAttribDesc.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyCI{};
        inputAssemblyCI.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyCI.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyCI.primitiveRestartEnable = VK_FALSE;

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicStateCI{};
        dynamicStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateCI.dynamicStateCount = static_cast<u32>(dynamicStates.size());
        dynamicStateCI.pDynamicStates = dynamicStates.data();

        // ClVkSwapchain* Swapchain = ClVkSwapchain::Get();
        // VkExtent2D swapchainExtent = *Swapchain->GetExtent();

        // VkViewport viewport{};
        // viewport.x = 0.0f;
        // viewport.y = 0.0f;
        // viewport.width = (f32)swapchainExtent.width;
        // viewport.height = (f32)swapchainExtent.height;
        // viewport.minDepth = 0.0f;
        // viewport.maxDepth = 1.0f;

        // VkRect2D scissor{};
        // scissor.offset = {0, 0};
        // scissor.extent = swapchainExtent;

        VkPipelineViewportStateCreateInfo viewportStateCI{};
        viewportStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportStateCI.viewportCount = 1;
        // viewportStateCI.pViewports = &viewport;
        viewportStateCI.scissorCount = 1;
        // viewportStateCI.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizerStateCI{};
        rasterizerStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerStateCI.depthClampEnable = VK_FALSE;
        rasterizerStateCI.rasterizerDiscardEnable = VK_FALSE;
        rasterizerStateCI.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizerStateCI.lineWidth = 1.0f;
        rasterizerStateCI.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizerStateCI.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        // rasterizerStateCI.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizerStateCI.depthBiasEnable = VK_FALSE;
        rasterizerStateCI.depthBiasConstantFactor = 0.0f;
        rasterizerStateCI.depthBiasClamp = 0.0f;
        rasterizerStateCI.depthBiasSlopeFactor = 0.0f;

        VkPipelineMultisampleStateCreateInfo multisamplingStateCI{};
        multisamplingStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisamplingStateCI.sampleShadingEnable = VK_FALSE;
        multisamplingStateCI.rasterizationSamples = MsaaSampleCount;
        multisamplingStateCI.minSampleShading = 1.0f;
        multisamplingStateCI.pSampleMask = nullptr;
        multisamplingStateCI.alphaToCoverageEnable = VK_FALSE;
        multisamplingStateCI.alphaToOneEnable = VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo depthStencilStateCI{};
        depthStencilStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilStateCI.depthTestEnable = VK_TRUE;
        depthStencilStateCI.depthWriteEnable = VK_TRUE;
        depthStencilStateCI.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencilStateCI.depthBoundsTestEnable = VK_FALSE;
        depthStencilStateCI.minDepthBounds = 0.0f;
        depthStencilStateCI.maxDepthBounds = 1.0f;
        depthStencilStateCI.stencilTestEnable = VK_FALSE;
        depthStencilStateCI.front = {};
        depthStencilStateCI.back = {};

        VkPipelineColorBlendAttachmentState colorBlendAtttachment{};
        colorBlendAtttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAtttachment.blendEnable = VK_FALSE;
        colorBlendAtttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAtttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAtttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAtttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAtttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAtttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlendStateCI{};
        colorBlendStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendStateCI.logicOpEnable = VK_FALSE;
        colorBlendStateCI.logicOp = VK_LOGIC_OP_COPY;
        colorBlendStateCI.attachmentCount = 1;
        colorBlendStateCI.pAttachments = &colorBlendAtttachment;
        colorBlendStateCI.blendConstants[0] = 0.0f;
        colorBlendStateCI.blendConstants[1] = 0.0f;
        colorBlendStateCI.blendConstants[2] = 0.0f;
        colorBlendStateCI.blendConstants[3] = 0.0f;

        // VkPipelineLayoutCreateInfo layoutCI{};
        // layoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        // if(DescSetLayout)
        // {
        //     layoutCI.setLayoutCount = 1;
        //     layoutCI.pSetLayouts = DescSetLayout;
        // }
        // else
        // {
        //     layoutCI.setLayoutCount = 0;
        //     layoutCI.pSetLayouts = nullptr;
        // }
        // layoutCI.pushConstantRangeCount = 0;
        // layoutCI.pPushConstantRanges = nullptr;

        ClVkDevice* Device = ClVkDevice::Get();
        // VK_CHECK(Device->CreatePiplineLayout(&layoutCI, nullptr, &m_layout));

        auto triangleShaderStages = depthShader.GetShaderStages();

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = static_cast<u32>(triangleShaderStages.size());
        pipelineInfo.pStages = triangleShaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputCI;
        pipelineInfo.pInputAssemblyState = &inputAssemblyCI;
        pipelineInfo.pViewportState = &viewportStateCI;
        pipelineInfo.pRasterizationState = &rasterizerStateCI;
        pipelineInfo.pMultisampleState = &multisamplingStateCI;
        pipelineInfo.pDepthStencilState = &depthStencilStateCI;
        pipelineInfo.pColorBlendState = &colorBlendStateCI;
        pipelineInfo.pDynamicState = &dynamicStateCI;
        // pipelineInfo.layout = m_layout;
        pipelineInfo.layout = *pPipelineLayout;
        pipelineInfo.renderPass = RenderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        // pipelineInfo.basePipelineIndex = -1;

        VK_CHECK(Device->CreateGraphicsPipeline(VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline));

        depthShader.Destroy();

        CLOG_INFO("VK Pipelines Created.");
    }

    void ClVkGraphicsPipeline::Destroy()
    {
        CLOG_INFO("Destroying Vk Pipelines...");

        ClVkDevice* Device = ClVkDevice::Get();
        Device->DestroyPipeline(m_pipeline, nullptr);
        // Device->DestroyPipelineLayout(m_layout, nullptr); 

        CLOG_INFO("Vk Pipelines Destroyed.");
    }

}

