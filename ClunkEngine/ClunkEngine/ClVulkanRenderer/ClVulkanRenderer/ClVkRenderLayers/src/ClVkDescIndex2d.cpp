#include "ClVkDescIndex2d.h"

#include <ClVulkan/VkUtils.h>

namespace Clunk::Vk
{

    ClVkDescIndex2dLayer::ClVkDescIndex2dLayer(ClVkContext &VkCtx)
    {
        std::vector<String> texture_files;
        for(uint32_t i = 0; i != 100; i++)
        {
            char fname[1024];
            snprintf(fname, sizeof(fname), "./Assets/textures/explosion0/explosion00-frame%03u.tga", i + 1);
            texture_files.push_back(fname);
        }

        const size_t num_texture_files = texture_files.size();
        mTextures.resize(num_texture_files);
        mSamplers.resize(num_texture_files);

        for(size_t i = 0; i < num_texture_files; i++)
        {
            printf("\rLoading texture %u...", unsigned(i));
            cl_create_vk_image(VkCtx, texture_files[i].c_str());
        }
        printf("\n");
        
        ClVkRenderPassInfo renderpass_info =
        {
            .bUseColor = true,
            .bClearColor = false,
            .bUseDepth = false,
            .bClearDepth = false,
            .ColorFormat = VkCtx.Swapchain.Format,
            .Flags = ERenderPassBit::ERPB_NONE,
            .Samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT
        };
        mRenderPass = cl_create_vk_renderpass(VkCtx, renderpass_info);
        mFramebuffers = cl_create_vk_color_only_framebuffers(VkCtx, mRenderPass);

        CreateDescriptor(VkCtx);

        VK_CHECK(create_vk_pipeline_layout(VkCtx.Device, static_cast<u32>(mDescriptor.Layouts.size()), mDescriptor.Layouts.data(), 0, nullptr, &mPipelineLayout));
        
        
    }

    ClVkDescIndex2dLayer::~ClVkDescIndex2dLayer()
    {
    }

    void ClVkDescIndex2dLayer::Destroy(ClVkContext &VkCtx)
    {
    }

    void ClVkDescIndex2dLayer::Update(u32 CurrentIndex, f32 DeltaTime)
    {
    }

    void ClVkDescIndex2dLayer::DrawFrame(const ClVkContext &VkCtx, const VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {
    }

    void ClVkDescIndex2dLayer::CreateDescriptor(ClVkContext &VkCtx)
    {
        mDescriptor.Pool = cl_create_vk_desc_pool(VkCtx, 0, 0, 1);

        std::array<VkDescriptorSetLayoutBinding, 1> bindings = {
            get_vk_desc_set_layout_binding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
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

        mDescriptor.Layouts = std::vector<VkDescriptorSetLayout>(VkCtx.FrameSync.GetNumFramesInFlight(), layout);
        mDescriptor.Sets.reserve(mDescriptor.Layouts.size());
        VkDescriptorSetAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = mDescriptor.Pool,
            .descriptorSetCount = static_cast<u32>(mDescriptor.Layouts.size()),
            .pSetLayouts = mDescriptor.Layouts.data()
        };
        VK_CHECK(vkAllocateDescriptorSets(VkCtx.Device, &alloc_info, mDescriptor.Sets.data()));

        std::vector<VkDescriptorImageInfo> texture_descriptors(mTextures.size());
        for(size_t i = 0; i < mTextures.size(); i++)
        {
            texture_descriptors[i] = VkDescriptorImageInfo
            {
                .sampler = mSamplers[i],
                .imageView = mTextures[i].View,
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            };
        }

        for (u32 i = 0; i < VkCtx.FrameSync.GetNumFramesInFlight(); i++)
        {
            std::vector<VkWriteDescriptorSet> desc_writes = {

                VkWriteDescriptorSet
                {
                    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .dstSet = mDescriptor.Sets[i],
                    .dstBinding = 0,
                    .dstArrayElement = 0,
                    .descriptorCount = static_cast<uint32_t>(mTextures.size()),
                    .pImageInfo = texture_descriptors.data()
                }
            };

            vkUpdateDescriptorSets(VkCtx.Device, static_cast<u32>(desc_writes.size()), desc_writes.data(), 0, nullptr);
        }
    }

    void ClVkDescIndex2dLayer::CreatePipeline(const ClVkContext &VkCtx, std::vector<ClVkShaderModule> &ShaderModules, ClVkRenderPass &RenderPass, VkPipelineLayout &Layout, VkExtent2D CustomExtent)
    {
        CLOG_INFO("Creating VkDescIndex2d pipeline...");

        std::vector<VkPipelineShaderStageCreateInfo> shader_stage_infos;
        for (ClVkShaderModule shader_module : ShaderModules)
        {
            shader_stage_infos.push_back(cl_get_vk_pipeline_shader_stage_create_info(shader_module));
        }

        VkPipelineVertexInputStateCreateInfo vert_input_info = create_info_vk_pipeline_vertex_input();
        VkVertexInputBindingDescription vert_binding = DescIndex2dVertex::GetBindDesc();
        vert_input_info.vertexBindingDescriptionCount = 1;
        vert_input_info.pVertexBindingDescriptions = &vert_binding;
        std::array<VkVertexInputAttributeDescription, 3>  vert_attributes = DescIndex2dVertex::GetAttibDesc();
        vert_input_info.vertexAttributeDescriptionCount = static_cast<u32>(vert_attributes.size());
        vert_input_info.pVertexAttributeDescriptions = vert_attributes.data();

        VkPipelineInputAssemblyStateCreateInfo input_assembly_info = create_info_vk_pipeline_assembly(VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

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
                .extent = VkExtent2D {
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
            .pTessellationState = &tessellation_info,
            .pViewportState = &viewport_info,
            .pRasterizationState = &rasterizer_info,
            .pMultisampleState = &multisample_info,
            .pDepthStencilState = nullptr,
            .pColorBlendState = &color_blend_info,
            .pDynamicState = &dynamic_info,
            .layout = mPipelineLayout,
            .renderPass = RenderPass.Handle,
            .subpass = 0,
            .basePipelineHandle = nullptr,
            .basePipelineIndex = -1
        };

        VK_CHECK(vkCreateGraphicsPipelines(VkCtx.Device, nullptr, 1, &create_info, nullptr, &mPipeline));
    }

    void ClVkDescIndex2dLayer::Draw(const ClVkContext &VkCtx, VkCommandBuffer CmdBuffer)
    {
    }
    
}