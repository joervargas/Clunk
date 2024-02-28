#include "ClVkRenderLayers/ClVkSimpleSkyBoxLayer.h"
#include <ClVulkan/VkUtils.h>
#include <ClVulkan/ClVkBuffer.h>
#include <ClMath/ClMath.h>

namespace Clunk::Vk
{
    const std::vector<SimpleSkyBoxVertex> SKY_VERTS = {
        { Vec3(-1.0f, -1.0f, 1.0f) },
        { Vec3( 1.0f, -1.0f, 1.0f) },
        { Vec3( 1.0f,  1.0f, 1.0f) },
        { Vec3(-1.0f,  1.0f, 1.0f) },

        { Vec3(-1.0f, -1.0f, -1.0) },
        { Vec3( 1.0f, -1.0f, -1.0) },
        { Vec3( 1.0f,  1.0f, -1.0) },
        { Vec3(-1.0f,  1.0f, -1.0) }
    };

    const std::vector<u32> SKY_INDICES = {
        // front
        0, 1, 2, 2, 3, 0,
        // right
        1, 5, 6, 6, 2, 1,
        // back
        7, 6, 5, 5, 4, 7,
        // left
        4, 0, 3, 3, 7, 4,
        // bottom
        4, 5, 1, 1, 0, 4,
        // top
        3, 2, 6, 6, 7, 3
    };

    Mat4 GetZUpMatrix()
    {
        f32 s = sin(Math::DegToRad(90.0f));
        f32 c = cos(Math::DegToRad(90.0f));
        return(Mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f,  c,    s,   0.0f,
            0.0f, -s,    c,   0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ));
    }

    ClVkSimpleSkyBoxLayer::ClVkSimpleSkyBoxLayer(ClVkContext &VkCtx, const ClVkImage &DepthImage, const ClVkBuffer &TransformUniform, std::vector<const char*> TextureFiles) :
        ClVk3dLayer()
    {
        mTexture = cl_create_vk_cubemap_image(VkCtx, TextureFiles);
        // TODO: check texture is not null

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

        
        mModelSpace = GetZUpMatrix();
        mModelSpaceBuffer = cl_create_vk_uniform_buffer<Mat4>(VkCtx, mModelSpace);
        
        CreateDescriptor(VkCtx, TransformUniform);

        VK_CHECK(create_vk_pipeline_layout(VkCtx.Device, static_cast<u32>(mDescriptor.Layouts.size()), mDescriptor.Layouts.data(), 0, nullptr, &mPipelineLayout));

        std::vector<ClVkShaderModule> shader_modules = {
            cl_create_vk_shader_module(VkCtx.Device, "./Shaders/GLSL/SimpleSkyBox.vert"),
            cl_create_vk_shader_module(VkCtx.Device, "./Shaders/GLSL/SimpleSkyBox.frag")
        };
        CreatePipeline(VkCtx, shader_modules, mRenderPass, mPipelineLayout);
        for(ClVkShaderModule shader : shader_modules)
        {
            cl_destroy_vk_shader_module(VkCtx.Device, shader);
        }

        mVerts = cl_create_vk_gpu_array_buffer<SimpleSkyBoxVertex>(VkCtx, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, SKY_VERTS);
        mIndices = cl_create_vk_gpu_array_buffer<u32>(VkCtx, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, SKY_INDICES);

        cl_update_vk_buffer(VkCtx, mModelSpaceBuffer, &mModelSpace, sizeof(Mat4));
    }

    ClVkSimpleSkyBoxLayer::~ClVkSimpleSkyBoxLayer()
    {
    }

    void ClVkSimpleSkyBoxLayer::Destroy(ClVkContext &VkCtx)
    {
        ClVk3dLayer::Destroy(VkCtx);

        cl_destroy_vk_buffer(VkCtx, mVerts);
        cl_destroy_vk_buffer(VkCtx, mIndices);
        cl_destroy_vk_buffer(VkCtx, mModelSpaceBuffer);
        cl_destroy_vk_image(VkCtx, &mTexture);
        vkDestroySampler(VkCtx.Device, mSampler, nullptr);
    }

    void ClVkSimpleSkyBoxLayer::Update(ClVkContext &VkCtx, u32 CurrentIndex, ClVkBuffer &ProjViewUniform, const ClProjectionView &ProjView, f32 DeltaTime)
    {
        // cl_update_vk_buffer(VkCtx, ProjViewUniform, &ProjView, sizeof(ClProjectionView));
        cl_update_vk_buffer(VkCtx, mModelSpaceBuffer, &mModelSpace, sizeof(Mat4));
    }

    void ClVkSimpleSkyBoxLayer::DrawFrame(const ClVkContext &VkCtx, const VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {   
        BeginRenderPass(VkCtx, CmdBuffer, CurrentImage);
        Draw(VkCtx, CmdBuffer);
        EndRenderPass(CmdBuffer);
    }

    void ClVkSimpleSkyBoxLayer::CreateDescriptor(ClVkContext &VkCtx, const ClVkBuffer &TransformUniform)
    {
        u32 NumFrames = VkCtx.FrameSync.GetNumFramesInFlight();
        mDescriptor.Pool = cl_create_vk_desc_pool(VkCtx, 2 * NumFrames, 0, 1 * NumFrames);

        std::vector<VkDescriptorSetLayoutBinding> bindings = {
            get_vk_desc_set_layout_binding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT),
            get_vk_desc_set_layout_binding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT),
            get_vk_desc_set_layout_binding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
        };
        // std::vector<VkDescriptorSetLayoutBinding> bindings = {
        //     get_vk_desc_set_layout_binding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT),
        //     get_vk_desc_set_layout_binding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
        // };

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
            VkDescriptorBufferInfo buffer_info2 = {
                .buffer = mModelSpaceBuffer.Handle,
                .offset = 0,
                .range = mModelSpaceBuffer.Size
            };
            VkDescriptorImageInfo img_info1 = {
                .sampler = mSampler,
                .imageView = mTexture.View,
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            };

            std::vector<VkWriteDescriptorSet> desc_writes = {
                get_vk_buffer_write_desc_set(mDescriptor.Sets[i], buffer_info1, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
                get_vk_buffer_write_desc_set(mDescriptor.Sets[i], buffer_info2, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
                get_vk_image_write_desc_set(mDescriptor.Sets[i], img_info1, 2)
                // get_vk_image_write_desc_set(mDescriptor.Sets[i], img_info1, 1)
            };

            vkUpdateDescriptorSets(VkCtx.Device, static_cast<u32>(desc_writes.size()), desc_writes.data(), 0, nullptr);
        }
    }

    void ClVkSimpleSkyBoxLayer::CreatePipeline(const ClVkContext &VkCtx, std::vector<ClVkShaderModule> &ShaderModules, ClVkRenderPass &RenderPass, VkPipelineLayout &Layout, VkExtent2D CustomExtent)
    {
        CLOG_INFO("Creating VkSimpleSkyBoxLayer pipeline...");

        std::vector<VkPipelineShaderStageCreateInfo> shader_stage_infos;
        for(ClVkShaderModule shader_module : ShaderModules)
        {
            shader_stage_infos.push_back(cl_get_vk_pipeline_shader_stage_create_info(shader_module));
        }

        VkPipelineVertexInputStateCreateInfo vert_input_info = create_info_vk_pipeline_vertex_input();
        VkVertexInputBindingDescription vert_binding = SimpleSkyBoxVertex::GetBindDesc();
        vert_input_info.vertexBindingDescriptionCount = 1;
        vert_input_info.pVertexBindingDescriptions = &vert_binding;
        std::array<VkVertexInputAttributeDescription, 1> vert_attributes = SimpleSkyBoxVertex::GetAttibDesc();
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
            VkCullModeFlagBits::VK_CULL_MODE_NONE,
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

        CLOG_INFO("VkSimpleSkyBoxLayer pipeline created.");
    }
    
    void ClVkSimpleSkyBoxLayer::Draw(const ClVkContext &VkCtx, VkCommandBuffer CmdBuffer)
    {
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(CmdBuffer, 0, 1, &mVerts.Handle, offsets);
        vkCmdBindIndexBuffer(CmdBuffer, mIndices.Handle, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0, 1, &mDescriptor.Sets[VkCtx.FrameSync.GetCurrentIndex()], 0, nullptr);

        // vkCmdDrawIndexed(CmdBuffer, static_cast<u32>(mIndices.Size), 1, 0, 0, 0);
        // u32 size = static_cast<u32>(mIndices.Size) / sizeof(u32);
        vkCmdDrawIndexed(CmdBuffer, mIndices.Count, 1, 0, 0, 0); 
        // vkCmdDraw(CmdBuffer, 36, 1, 0, 0);
    }
}