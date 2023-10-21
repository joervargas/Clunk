#include "ClVkRenderLayers/ClVkSimple2dLayer.h"
#include "ClVkSimple2dLayer.h"

#include <ClVulkan/VkUtils.h>

namespace Clunk::Vk
{
    ClVkSimple2dLayer::ClVkSimple2dLayer(ClVkContext *pVkCtx) :
        ClVk2dLayer(pVkCtx)
    {
        ClVkRenderPassInfo renderpass_info =
        {
            .bUseColor = true,
            .bClearColor = false,
            .bUseDepth = true,
            .bClearDepth = false,
            .ColorFormat = VkFormat::VK_FORMAT_B8G8R8A8_UNORM,
            .Flags = ERenderPassBit::ERPB_NONE,
            .Samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT
        };
        mRenderPass = cl_create_vk_renderpass(*pVkCtx, renderpass_info);
        mFramebuffers = cl_create_vk_color_only_framebuffers(*pVkCtx, mRenderPass);

        VK_CHECK(create_vk_pipeline_layout(pVkCtx->Device, 0, nullptr, 0, nullptr, &mPipelineLayout));

        std::vector<ClVkShaderModule> shader_modules = {
            cl_create_vk_shader_module(pVkCtx->Device, "./Shaders/GLSL/Simple2dLayer.vert"),
            cl_create_vk_shader_module(pVkCtx->Device, "./Shaders/GLSL/Simple2dLayer.frag")
        };

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

    void ClVkSimple2dLayer::CreatePipeline(std::vector<ClVkShaderModule> ShaderModules, ClVkRenderPass RenderPass, VkPipelineLayout Layout, VkExtent2D CustomExtent)
    {
        CLOG_INFO("Creating VkSimple2dLayer pipeline...");

        std::vector<VkPipelineShaderStageCreateInfo> shader_stage_infos;
        for (ClVkShaderModule shader_module : ShaderModules)
        {
            shader_stage_infos.push_back(cl_get_vk_pipeline_shader_stage_create_info(shader_module));
        }

        VkPipelineVertexInputStateCreateInfo vert_input_info = create_info_vk_pipeline_vertex_input();

        VkPipelineInputAssemblyStateCreateInfo input_assembly_info = create_info_vk_pipeline_assembly(VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

        std::array<VkViewport, 1> viewports = {
            VkViewport
            {
                .x = 0.0f,
                .y = 0.0f,
                .width = CustomExtent.width > 0 ? static_cast<f32>(CustomExtent.width) : static_cast<f32>(pVkCtx->Swapchain.Width),
                .height = CustomExtent.height > 0 ? static_cast<f32>(CustomExtent.height) : static_cast<f32>(pVkCtx->Swapchain.Height),
                .minDepth = 0.0f,
                .maxDepth = 1.0f
            }
        };
        std::array<VkRect2D, 1> scissors = {
            VkRect2D
            {
                .offset = VkOffset2D{ .x = 0, .y = 0 },
                .extent = VkExtent2D {
                    .width = CustomExtent.width > 0 ? CustomExtent.width : pVkCtx->Swapchain.Width,
                    .height = CustomExtent.height > 0 ? CustomExtent.height : pVkCtx->Swapchain.Height
                }
            }
        };
        VkPipelineViewportStateCreateInfo viewport_info = create_info_vk_pipeline_viewport(
            static_cast<u32>(viewports.size()), viewports.data(), 
            static_cast<u32>(scissors.size()), scissors.data()
        );

        VkPipelineRasterizationStateCreateInfo rasterizer_info = create_info_vk_pipeline_rasterization(
            VkPolygonMode::VK_POLYGON_MODE_FILL, 
            VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT, 
            VkFrontFace::VK_FRONT_FACE_CLOCKWISE, 
            1.0f
        );

        VkPipelineMultisampleStateCreateInfo multisample_info = create_info_vk_pipeline_multisample(VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VK_FALSE);

        std::array<VkPipelineColorBlendAttachmentState, 1> color_blend_attachments = {
            create_info_vk_pipeline_color_blend_attachment(true)
        };
        VkPipelineColorBlendStateCreateInfo color_blend_info = create_info_vk_pipeline_color_blend(color_blend_attachments.data(), static_cast<u32>(color_blend_attachments.size()) );

        std::array<VkDynamicState, 2> dynamic_states = {
            VkDynamicState::VK_DYNAMIC_STATE_VIEWPORT,
            VkDynamicState::VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamic_info = create_info_vk_pipeline_dynamic_state(dynamic_states.data(), static_cast<u32>(dynamic_states.size()) );
        
        VkPipelineTessellationStateCreateInfo tessellation_info = create_info_vk_pipeline_tessellation(0);

        VkGraphicsPipelineCreateInfo create_info =
        {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stageCount = static_cast<u32>(shader_stage_infos.size()),
            .pStages = shader_stage_infos.data(),
            .pVertexInputState = &vert_input_info,
            .pInputAssemblyState = &input_assembly_info,
            .pViewportState = &viewport_info,
            .pRasterizationState = &rasterizer_info,
            .pMultisampleState = &multisample_info,
            .pColorBlendState = &color_blend_info,
            .pDepthStencilState = nullptr,
            .pDynamicState = &dynamic_info,
            .pTessellationState = &tessellation_info,
            .layout = mPipelineLayout,
            .renderPass = RenderPass.Handle,
            .subpass = 0,
            .basePipelineHandle = nullptr,
            .basePipelineIndex = -1
        };

        VK_CHECK(vkCreateGraphicsPipelines(pVkCtx->Device, nullptr, 1, &create_info, nullptr, &mPipeline));
    }

    void ClVkSimple2dLayer::Draw(VkCommandBuffer CmdBuffer)
    {
        vkCmdDraw(CmdBuffer, 3, 1, 0, 0);
    }
}