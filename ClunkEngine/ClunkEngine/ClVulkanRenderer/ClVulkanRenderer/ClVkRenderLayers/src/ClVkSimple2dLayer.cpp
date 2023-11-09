#include "ClVkRenderLayers/ClVkSimple2dLayer.h"
#include "ClVkSimple2dLayer.h"

#include <ClVulkan/VkUtils.h>

namespace Clunk::Vk
{
    ClVkSimple2dLayer::ClVkSimple2dLayer(ClVkContext& VkCtx, const char* TextureFile)
    {
        mTexture = cl_create_vk_image(VkCtx, TextureFile);
        create_vk_sampler(VkCtx.Device, &mSampler);

        ClVkRenderPassInfo renderpass_info =
        {
            .bUseColor = true,
            .bClearColor = false,
            .bUseDepth = false,
            .bClearDepth = false,
            // .ColorFormat = VkFormat::VK_FORMAT_B8G8R8A8_UNORM,
            .ColorFormat = VkCtx.Swapchain.Format,
            .Flags = ERenderPassBit::ERPB_NONE,
            .Samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT
        };
        mRenderPass = cl_create_vk_renderpass(VkCtx, renderpass_info);
        // cl_create_vk_renderpass(*pVkCtx, renderpass_info, &mRenderPass);
        mFramebuffers = cl_create_vk_color_only_framebuffers(VkCtx, mRenderPass);
        CreateDescriptor(VkCtx);

        VK_CHECK(create_vk_pipeline_layout(VkCtx.Device, static_cast<u32>(mDescriptor.Layouts.size()), mDescriptor.Layouts.data(), 0, nullptr, &mPipelineLayout));

        std::vector<ClVkShaderModule> shader_modules = {
            cl_create_vk_shader_module(VkCtx.Device, "./Shaders/GLSL/Simple2dLayer.vert"),
            cl_create_vk_shader_module(VkCtx.Device, "./Shaders/GLSL/Simple2dLayer.frag")
        };
        CreatePipeline(VkCtx, shader_modules, mRenderPass, mPipelineLayout);

        for(ClVkShaderModule shader : shader_modules)
        {
            cl_destroy_vk_shader_module(VkCtx.Device, shader);
        }

        // create vertex buffer
        VkDeviceSize vbuffer_size = sizeof(Simple2dVertex) * VERTICES_DATA.size();
        ClVkBuffer staging_triangle_vert = cl_create_vk_buffer(VkCtx, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT, vbuffer_size);

        void* data_ptr = nullptr;
        vmaMapMemory(VkCtx.MemAllocator, staging_triangle_vert.Allocation, &data_ptr);
            memcpy(data_ptr, VERTICES_DATA.data(), (size_t)vbuffer_size);
        vmaUnmapMemory(VkCtx.MemAllocator, staging_triangle_vert.Allocation);

        mVerts = cl_create_vk_buffer(VkCtx, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 0, vbuffer_size);
        VkCommandBuffer cmd_buffer = cl_begin_single_time_vk_command_buffer(VkCtx);
            copy_vk_buffer(VkCtx.Device, cmd_buffer, staging_triangle_vert.Handle, mVerts.Handle, vbuffer_size);
        cl_end_single_time_vk_command_buffer(VkCtx, cmd_buffer);

        cl_destroy_vk_buffer(VkCtx, staging_triangle_vert);

        // create index buffer
        VkDeviceSize ibuffer_size = sizeof(u16) * INDICE_DATA.size();
        ClVkBuffer staging_triangle_indices = cl_create_vk_buffer(VkCtx, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT, ibuffer_size);

        data_ptr = nullptr;
        vmaMapMemory(VkCtx.MemAllocator, staging_triangle_indices.Allocation, &data_ptr);
            memcpy(data_ptr, INDICE_DATA.data(), (size_t)ibuffer_size);
        vmaUnmapMemory(VkCtx.MemAllocator, staging_triangle_indices.Allocation);

        mIndices = cl_create_vk_buffer(VkCtx, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 0, ibuffer_size);
        cmd_buffer = cl_begin_single_time_vk_command_buffer(VkCtx);
            copy_vk_buffer(VkCtx.Device, cmd_buffer, staging_triangle_indices.Handle, mIndices.Handle, ibuffer_size);
        cl_end_single_time_vk_command_buffer(VkCtx, cmd_buffer);

        cl_destroy_vk_buffer(VkCtx, staging_triangle_indices);
    }

    ClVkSimple2dLayer::~ClVkSimple2dLayer()
    {
    }

    void ClVkSimple2dLayer::Destroy(ClVkContext &VkCtx)
    {
        ClVk2dLayer::Destroy(VkCtx);
        
        cl_destroy_vk_buffer(VkCtx, mVerts);
        cl_destroy_vk_buffer(VkCtx, mIndices);
        cl_destroy_vk_image(VkCtx, &mTexture);
        vkDestroySampler(VkCtx.Device, mSampler, nullptr);
    }

    void ClVkSimple2dLayer::Update(u32 CurrentIndex, f32 DeltaTime)
    {
    }

    void ClVkSimple2dLayer::DrawFrame(const ClVkContext& VkCtx, const VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {
        BeginRenderPass(VkCtx, CmdBuffer, CurrentImage);
        Draw(VkCtx, CmdBuffer);
        EndRenderPass(CmdBuffer);
    }

    void ClVkSimple2dLayer::CreateDescriptor(ClVkContext &VkCtx)
    {
        mDescriptor.Pool = cl_create_vk_desc_pool(VkCtx, 0, 0, 1);

        std::vector<VkDescriptorSetLayoutBinding> bindings = {
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

        for (u32 i = 0; i < VkCtx.FrameSync.GetNumFramesInFlight(); i++)
        {
            VkDescriptorImageInfo img_info1 = VkDescriptorImageInfo
            { 
                .sampler = mSampler,
                .imageView = mTexture.View,
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            };

            std::vector<VkWriteDescriptorSet> desc_writes = {
                get_vk_image_write_desc_set(mDescriptor.Sets[i], img_info1, 0)
            };

            vkUpdateDescriptorSets(VkCtx.Device, static_cast<u32>(desc_writes.size()), desc_writes.data(), 0, nullptr);
        }
    }

    void ClVkSimple2dLayer::CreatePipeline(const ClVkContext &VkCtx, std::vector<ClVkShaderModule> &ShaderModules, ClVkRenderPass &RenderPass, VkPipelineLayout &Layout, VkExtent2D CustomExtent)
    {
        CLOG_INFO("Creating VkSimple2dLayer pipeline...");

        std::vector<VkPipelineShaderStageCreateInfo> shader_stage_infos;
        for (ClVkShaderModule shader_module : ShaderModules)
        {
            shader_stage_infos.push_back(cl_get_vk_pipeline_shader_stage_create_info(shader_module));
        }

        VkPipelineVertexInputStateCreateInfo vert_input_info = create_info_vk_pipeline_vertex_input();
        VkVertexInputBindingDescription vert_binding = Simple2dVertex::GetBindDesc();
        vert_input_info.vertexBindingDescriptionCount = 1;
        vert_input_info.pVertexBindingDescriptions = &vert_binding;
        std::array<VkVertexInputAttributeDescription, 3>  vert_attributes = Simple2dVertex::GetAttibDesc();
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

    void ClVkSimple2dLayer::Draw(const ClVkContext& VkCtx, VkCommandBuffer CmdBuffer)
    {
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(CmdBuffer, 0, 1, &mVerts.Handle, offsets);
        vkCmdBindIndexBuffer(CmdBuffer, mIndices.Handle, 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0, 1, &mDescriptor.Sets[VkCtx.FrameSync.GetCurrentIndex()], 0, nullptr);

        // vkCmdDraw(CmdBuffer, static_cast<u32>(VERTICES_DATA.size()), 1, 0, 0);
        vkCmdDrawIndexed(CmdBuffer, static_cast<u32>(INDICE_DATA.size()), 1, 0, 0, 0);
    }
}