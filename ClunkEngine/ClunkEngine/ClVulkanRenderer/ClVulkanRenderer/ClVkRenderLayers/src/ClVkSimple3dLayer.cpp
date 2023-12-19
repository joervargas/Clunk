#include "ClVkRenderLayers/ClVkSimple3dLayer.h"
#include "ClVkSimple3dLayer.h"

#include <ClVulkan/VkUtils.h>


namespace Clunk::Vk
{
    ClVkSimple3dLayer::ClVkSimple3dLayer(ClVkContext &VkCtx, const ClVkImage& DepthImage, const ClVkBuffer& TransformUniform, const char* MeshFile, const char* TextureFile) :
        ClVk3dLayer()
    {
        mTexture = cl_create_vk_image(VkCtx, TextureFile);
        create_vk_sampler(VkCtx.Device, &mSampler);

        ClVkRenderPassInfo renderpass_info =
        {
            .bUseColor = true,
            .bClearColor = false,
            .bUseDepth = true,
            .bClearDepth = false,
            .ColorFormat = VkCtx.Swapchain.Format,
            .Flags = ERenderPassBit::ERPB_NONE,
            .Samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT
        };
        mRenderPass = cl_create_vk_renderpass(VkCtx, renderpass_info);
        mFramebuffers = cl_create_vk_color_depth_framebuffers(VkCtx, mRenderPass, DepthImage.View);
        CreateDescriptor(VkCtx, TransformUniform);

        VK_CHECK(create_vk_pipeline_layout(VkCtx.Device, static_cast<u32>(mDescriptor.Layouts.size()), mDescriptor.Layouts.data(), 0, nullptr, &mPipelineLayout));
        
        std::vector<ClVkShaderModule> shader_modules = {
            cl_create_vk_shader_module(VkCtx.Device, "./Shaders/GLSL/Simple3dLayer.vert"),
            cl_create_vk_shader_module(VkCtx.Device, "./Shaders/GLSL/Simple3dLayer.frag")
        };
        CreatePipeline(VkCtx, shader_modules, mRenderPass, mPipelineLayout);

        for(ClVkShaderModule shader : shader_modules)
        {
            cl_destroy_vk_shader_module(VkCtx.Device, shader);
        }


    }

    ClVkSimple3dLayer::~ClVkSimple3dLayer()
    {
    }

    void ClVkSimple3dLayer::Destroy(ClVkContext &VkCtx)
    {
        ClVk3dLayer::Destroy(VkCtx);
    }

    void ClVkSimple3dLayer::Update(u32 CurrentIndex, ClVkBuffer &TransformUniform, ClVkImage &DepthImage, f32 DeltaTime)
    {
    }

    void ClVkSimple3dLayer::DrawFrame(const ClVkContext &VkCtx, const VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {
    }

    void ClVkSimple3dLayer::CreateDescriptor(ClVkContext &VkCtx, const ClVkBuffer& TransformUniform)
    {
        u32 NumFrames = VkCtx.FrameSync.GetNumFramesInFlight();
        mDescriptor.Pool = cl_create_vk_desc_pool(VkCtx, 1 * NumFrames, 0, 1 * NumFrames);

        std::vector<VkDescriptorSetLayoutBinding> bindings = {
            get_vk_desc_set_layout_binding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT),
            get_vk_desc_set_layout_binding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
        };

        VkDescriptorSetLayoutCreateInfo layout_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .bindingCount = static_cast<u32>(bindings.size()),
            .pBindings = bindings.data()
        };
        VkDescriptorSetLayout layout = nullptr;
        VK_CHECK(vkCreateDescriptorSetLayout(VkCtx.Device, &layout_info, nullptr, &layout));

        mDescriptor.Layouts = std::vector<VkDescriptorSetLayout>(NumFrames, layout);
        mDescriptor.Sets.resize(mDescriptor.Layouts.size());
        VkDescriptorSetAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = mDescriptor.Pool,
            .descriptorSetCount = static_cast<u32>(mDescriptor.Layouts.size()),
            .pSetLayouts = mDescriptor.Layouts.data()
        };
        VK_CHECK(vkAllocateDescriptorSets(VkCtx.Device, &alloc_info, mDescriptor.Sets.data()));

        for(u32 i = 0; i < NumFrames; i++)
        {
            VkDescriptorBufferInfo buffer_info1 = {
                .buffer = TransformUniform.Handle,
                .offset = 0,
                .range = TransformUniform.Size
            };

            VkDescriptorImageInfo img_info1 = {
                .sampler = mSampler,
                .imageView = mTexture.View,
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            };

            std::vector<VkWriteDescriptorSet> desc_writes = {
                get_vk_buffer_write_desc_set(mDescriptor.Sets[i], buffer_info1, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
                get_vk_image_write_desc_set(mDescriptor.Sets[i], img_info1, 1)
            };

            vkUpdateDescriptorSets(VkCtx.Device, static_cast<u32>(desc_writes.size()), desc_writes.data(), 0, nullptr);
        }
    }

    void ClVkSimple3dLayer::CreatePipeline(const ClVkContext &VkCtx, std::vector<ClVkShaderModule> &ShaderModules, ClVkRenderPass &RenderPass, VkPipelineLayout &Layout, VkExtent2D CustomExtent)
    {
        CLOG_INFO("Creating VkSimple3dLayer pipeline...");

        std::vector<VkPipelineShaderStageCreateInfo> shader_stage_infos;
        for(ClVkShaderModule shader_module : ShaderModules)
        {
            shader_stage_infos.push_back(cl_get_vk_pipeline_shader_stage_create_info(shader_module));
        }

        VkPipelineVertexInputStateCreateInfo vert_input_info = create_info_vk_pipeline_vertex_input();
        VkVertexInputBindingDescription vert_binding = Simple3dVertex::GetBindDesc();
        vert_input_info.vertexBindingDescriptionCount = 1;
        vert_input_info.pVertexBindingDescriptions = &vert_binding;
        std::array<VkVertexInputAttributeDescription, 3> vert_attributes = Simple3dVertex::GetAttibDesc();
        vert_input_info.vertexAttributeDescriptionCount = static_cast<u32>(vert_attributes.size());
        vert_input_info.pVertexAttributeDescriptions = vert_attributes.data();

        VkPipelineInputAssemblyStateCreateInfo input_assembly_info = create_info_vk_pipeline_assembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

        std::array<VkViewport, 1> viewports = {
            VkViewport
            {
                .x = 0.0f,
                .y = 0.0f,
                .width = CustomExtent.width > 0 ? static_cast<f32>(CustomExtent.width) : static_cast<f32>(VkCtx.Swapchain.Width),
                .height = CustomExtent.height > 0 ? static_cast<f32>(CustomExtent.height) : static_cast<f32>(VkCtx.Swapchain.Height),
                .minDepth = 0.0f,
                .maxDepth = 1.0f
            }
        };
        std::array<VkRect2D, 1> scissors = {
            VkRect2D
            {
                .offset = VkOffset2D{ .x = 0, .y = 0 },
                .extent = VkExtent2D{
                    .width = CustomExtent.width > 0 ? CustomExtent.width : VkCtx.Swapchain.Width,
                    .height = CustomExtent.height > 0 ? CustomExtent.height : VkCtx.Swapchain.Height
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
        VkPipelineColorBlendStateCreateInfo color_blend_info = create_info_vk_pipeline_color_blend(color_blend_attachments.data(), static_cast<u32>(color_blend_attachments.size()));

        VkPipelineDepthStencilStateCreateInfo depth_stencil_info = create_info_vk_pipeline_depth_stencil();

        std::array<VkDynamicState, 2> dynamic_states = {
            VkDynamicState::VK_DYNAMIC_STATE_VIEWPORT,
            VkDynamicState::VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamic_info = create_info_vk_pipeline_dynamic_state(dynamic_states.data(), static_cast<u32>(dynamic_states.size()));

        VkPipelineTessellationStateCreateInfo tesselation_info = create_info_vk_pipeline_tessellation(0);

        VkGraphicsPipelineCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stageCount = static_cast<u32>(shader_stage_infos.size()),
            .pStages = shader_stage_infos.data(),
            .pVertexInputState = &vert_input_info,
            .pInputAssemblyState = &input_assembly_info,
            .pTessellationState = &tesselation_info,
            .pViewportState = &viewport_info,
            .pRasterizationState = &rasterizer_info,
            .pMultisampleState = &multisample_info,
            .pDepthStencilState = &depth_stencil_info,
            .pColorBlendState = &color_blend_info,
            .pDynamicState = &dynamic_info,
            .layout = Layout,
            .renderPass = RenderPass.Handle,
            .subpass = 0,
            .basePipelineHandle = nullptr,
            .basePipelineIndex = -1
        };

        VK_CHECK(vkCreateGraphicsPipelines(VkCtx.Device, nullptr, 1, &create_info, nullptr, &mPipeline));

        CLOG_INFO("VkSimple3dLayer pipeline created.");
    }
    
    void ClVkSimple3dLayer::Draw(const ClVkContext &VkCtx, VkCommandBuffer CmdBuffer)
    {
    }
}