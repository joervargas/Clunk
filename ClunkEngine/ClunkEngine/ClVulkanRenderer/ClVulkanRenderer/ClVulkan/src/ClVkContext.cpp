#include "ClVkContext.h"
#include "VkUtils.h"
#include "ClVkRenderPass.h"
#include "VkShaderUtils.h"

#include <array>

namespace Clunk::Vk
{

    void cl_query_vk_queues_indices(const VkPhysicalDevice PhysicalDevice, ClVkQueues *Queues)
    {
        if (!Queues) { THROW_EXCEPTION("VulkanQueus* must not be nullptr!"); }

        Queues->Graphics.index = find_vk_queue_family_index(PhysicalDevice, VK_QUEUE_GRAPHICS_BIT);
    }

    void cl_query_vk_queues_handles(const VkDevice Device, ClVkQueues *Queues)
    {
        if (!Queues) { THROW_EXCEPTION("VulkanQueus* must not be nullptr!"); }

        if(Queues->Graphics.index.has_value())
        {
            vkGetDeviceQueue(Device, Queues->Graphics.index.value(), 0, &Queues->Graphics.Handle);
        }
    }

    std::vector<u32> cl_get_vk_queues_indices_list(const ClVkQueues* Queues)
    {
        std::vector<u32> result;
        if (Queues->Graphics.index.has_value())
        {
            result.push_back(Queues->Graphics.index.value());
        }
        return result;
    }

    ClVkSwapchain cl_create_vk_swapchain(const VkDevice Device, VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface, std::vector<u32> QueueFamilyIndices, u32 Width, u32 Height)
    {
        ClVkSwapchain vkSwapchain;

        VkSwapchainDetails details = query_vk_swapchain_details(PhysicalDevice, Surface);
        VkSurfaceFormatKHR format = choose_vk_swap_surface_format(details.Formats);
        vkSwapchain.Format = format.format;
        VkPresentModeKHR presentMode = choose_vk_swap_present_mode(details.PresentModes);

        // vkSwapchain.Extent = { .width = Width, .height = Height };
        vkSwapchain.Width = Width;
        vkSwapchain.Height = Height;
        create_vk_swapchain(Device, Surface, QueueFamilyIndices, details.Capabilities, format, presentMode, vkSwapchain.Width, vkSwapchain.Height, &vkSwapchain.Handle);
        create_vk_swapchain_images(Device, vkSwapchain.Handle, vkSwapchain.Images, vkSwapchain.ImageViews);
        
        return vkSwapchain;
    }

    void cl_destroy_vk_swapchain(const VkDevice Device, ClVkSwapchain *pSwapchain)
    {
        for(size_t i = 0; i < pSwapchain->ImageViews.size(); i++)
        {
            vkDestroyImageView(Device, pSwapchain->ImageViews[i], nullptr);
        }
        vkDestroySwapchainKHR(Device, pSwapchain->Handle, nullptr);
        pSwapchain->Handle = nullptr;
    }

    ClVkFrameSync::ClVkFrameSync(VkDevice Device, u32 NumFramesInFlight)
    {
        FramesInFlight = NumFramesInFlight;
        CurrentFrameIndex = 0;
        for(u32 i = 0; i < FramesInFlight; i++)
        {
            VkSemaphore wait_semaphore;
            VK_CHECK(create_vk_semaphore(Device, &wait_semaphore));
            WaitSemaphores.push_back(wait_semaphore);

            VkSemaphore render_semaphore;
            VK_CHECK(create_vk_semaphore(Device, &render_semaphore));
            RenderSemaphores.push_back(render_semaphore);

            VkFence in_flight_fence;
            VK_CHECK(create_vk_fence(Device, &in_flight_fence, true));
            InFlightFences.push_back(in_flight_fence);
        }
    }

    void ClVkFrameSync::Destroy(VkDevice Device)
    {
        for(u32 i = 0; i < FramesInFlight; i++)
        {
            vkDestroySemaphore(Device, WaitSemaphores[i], nullptr);
            vkDestroySemaphore(Device, RenderSemaphores[i], nullptr);
            vkDestroyFence(Device, InFlightFences[i], nullptr);
        }
        FramesInFlight = 0;
        CurrentFrameIndex = 0;
        WaitSemaphores.clear();
        RenderSemaphores.clear();
        InFlightFences.clear();
    }

    ClVkContext cl_create_vk_context(const ClVkLoader &Loader, u32 Width, u32 Height)
    {
        ClVkContext ctx;

        VK_CHECK( find_suitable_vk_physical_device(Loader.Instance, Loader.Surface, &ctx.PhysicalDevice) );

        cl_query_vk_queues_indices(ctx.PhysicalDevice, &ctx.Queues);
        std::vector<u32> queue_indices = cl_get_vk_queues_indices_list(&ctx.Queues);
        
        create_vk_device(ctx.PhysicalDevice, queue_indices, &ctx.Device);
        cl_query_vk_queues_handles(ctx.Device, &ctx.Queues);

        create_vk_mem_allocator(Loader.Instance, Loader.Vk_API_Version, ctx.PhysicalDevice, ctx.Device, &ctx.MemAllocator );

        ctx.Swapchain = cl_create_vk_swapchain(ctx.Device, ctx.PhysicalDevice, Loader.Surface, queue_indices, Width, Height);
        
        // VK_CHECK(create_vk_semaphore(ctx.Device, &ctx.RenderSemaphore));
        // VK_CHECK(create_vk_semaphore(ctx.Device, &ctx.WaitSemaphore));
        ctx.FrameSync = ClVkFrameSync(ctx.Device, 2);

        VK_CHECK( create_vk_command_pool(ctx.Device, ctx.Queues.Graphics.index.value(), &ctx.DrawCmds.Pool));
        VK_CHECK( allocate_vk_command_buffers(ctx.Device, ctx.DrawCmds.Pool, static_cast<u32>( ctx.Swapchain.Images.size()), ctx.DrawCmds.Buffers) );

        return ctx;
    }

    void cl_destroy_vk_context(ClVkContext* VkCtx)
    {

        cl_destroy_vk_swapchain(VkCtx->Device, &VkCtx->Swapchain);
        vkDestroyCommandPool(VkCtx->Device, VkCtx->DrawCmds.Pool, nullptr);

        // vkDestroySemaphore(VkCtx->Device, VkCtx->RenderSemaphore, nullptr);
        // vkDestroySemaphore(VkCtx->Device, VkCtx->WaitSemaphore, nullptr);
        VkCtx->FrameSync.Destroy(VkCtx->Device);
           
        vmaDestroyAllocator(VkCtx->MemAllocator);
        vkDestroyDevice(VkCtx->Device, nullptr);
    }


    VkCommandBuffer cl_begin_single_time_vk_command_buffer(ClVkContext &VkCtx)
    {
        VkCommandBuffer cmdBuffer;

        const VkCommandBufferAllocateInfo allocInfo =
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = VkCtx.DrawCmds.Pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };
        VK_CHECK(vkAllocateCommandBuffers(VkCtx.Device, &allocInfo, &cmdBuffer));

        const VkCommandBufferBeginInfo beginInfo =
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            .pInheritanceInfo = nullptr
        };
        VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

        return cmdBuffer;
    }

    void cl_end_single_time_vk_command_buffer(ClVkContext &VkCtx, VkCommandBuffer CmdBuffer)
    {
        vkEndCommandBuffer(CmdBuffer);
        
        const VkSubmitInfo submitInfo =
        {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &CmdBuffer,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr
        };
        vkQueueSubmit(VkCtx.Queues.Graphics.Handle, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(VkCtx.Queues.Graphics.Handle);

        vkFreeCommandBuffers(VkCtx.Device, VkCtx.DrawCmds.Pool, 1, &CmdBuffer);
    }

    VkPipeline cl_create_vk_graphics_pipeline(ClVkContext &VkCtx, ClVkRenderPass RenderPass, VkPipelineLayout PipelineLayout, const std::vector<const char *> &ShaderFiles, VkPrimitiveTopology Topology, bool bDynamicScissor, bool bUseBlending, i32 CustomWidth, i32 CustomHeight, u32 NumPatchControlPoints)
    {
        std::vector<ClVkShaderModule> shader_modules;
        std::vector<VkPipelineShaderStageCreateInfo> shader_stages;

        shader_modules.resize(ShaderFiles.size());
        shader_stages.resize(ShaderFiles.size());

        for(size_t i = 0; i < ShaderFiles.size(); i++)
        {
            const char* file = ShaderFiles[i];
            shader_modules[i] = cl_create_vk_shader_module(VkCtx.Device, file);

            VkShaderStageFlagBits stage = get_vk_shader_stage_from_filename(file);

            shader_stages[i] = cl_get_vk_shader_stage_create_info(shader_modules[i], stage);
        }

        const VkPipelineVertexInputStateCreateInfo vertex_input_state_info =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
        };

        const VkPipelineInputAssemblyStateCreateInfo input_assembly_state_info =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = Topology,
            .primitiveRestartEnable = VK_FALSE
        };

        const VkViewport viewport =
        {
            .x = 0.f,
            .y = 0.f,
            .width = static_cast<f32>(CustomWidth > 0 ? CustomWidth : VkCtx.Swapchain.Width),
            .height = static_cast<f32>(CustomHeight > 0 ? CustomHeight : VkCtx.Swapchain.Height),
            .minDepth = 0.f,
            .maxDepth = 1.0f
        };

        const VkRect2D scissor =
        {
            .offset = { 0, 0 },
            .extent =
            {
                CustomWidth > 0 ? CustomWidth : VkCtx.Swapchain.Width,
                CustomHeight > 0 ? CustomHeight : VkCtx.Swapchain.Height
            }
        };

        const VkPipelineViewportStateCreateInfo viewport_state_info =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor
        };

        const VkPipelineRasterizationStateCreateInfo rasterizer_state_info =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_BACK_BIT,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .lineWidth = 1.0f
        };
        
        const VkPipelineMultisampleStateCreateInfo multi_sample_state_info =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = VK_FALSE,
            .minSampleShading = 1.0f
        };

        const VkPipelineColorBlendAttachmentState color_blend_attachment_state =
        {
            .blendEnable = VK_TRUE,
            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            .colorBlendOp = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = bUseBlending ? VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA : VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp = VK_BLEND_OP_ADD,
            .colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT |
                VK_COLOR_COMPONENT_G_BIT |
                VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT
        };

        const VkPipelineColorBlendStateCreateInfo color_blend_state_info =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &color_blend_attachment_state,
            .blendConstants = { 0.f, 0.f, 0.f, 0.f }
        };

        const VkPipelineDepthStencilStateCreateInfo depth_stencil_state_info =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .depthTestEnable = RenderPass.Info.bUseDepth ? VK_TRUE : VK_FALSE,
            .depthWriteEnable = RenderPass.Info.bUseDepth ? VK_TRUE : VK_FALSE,
            .depthCompareOp = VK_COMPARE_OP_LESS,
            .depthBoundsTestEnable = VK_FALSE,
            .minDepthBounds = 0.0f,
            .maxDepthBounds = 1.0f
        };

        std::array<VkDynamicState, 1> dynamic_states = { VK_DYNAMIC_STATE_SCISSOR };
        const VkPipelineDynamicStateCreateInfo dynamic_state_info =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .dynamicStateCount = static_cast<u32>(dynamic_states.size()),
            .pDynamicStates = dynamic_states.data()
        };

        const VkPipelineTessellationStateCreateInfo tessellation_state_info =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .patchControlPoints = NumPatchControlPoints
        };

        const VkGraphicsPipelineCreateInfo create_info =
        {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = static_cast<u32>(shader_stages.size()),
            .pStages = shader_stages.data(),
            .pVertexInputState = &vertex_input_state_info,
            .pInputAssemblyState = &input_assembly_state_info,
            .pTessellationState = &tessellation_state_info,
            .pViewportState = &viewport_state_info,
            .pRasterizationState = &rasterizer_state_info,
            .pMultisampleState = &multi_sample_state_info,
            .pDepthStencilState = &depth_stencil_state_info,
            .pColorBlendState = &color_blend_state_info,
            .pDynamicState = bDynamicScissor ? &dynamic_state_info : nullptr,
            .layout = PipelineLayout,
            .renderPass = RenderPass.Handle,
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = -1
        };
        VkPipeline pipeline;
        VK_CHECK(vkCreateGraphicsPipelines(VkCtx.Device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline))
        
        for(ClVkShaderModule module : shader_modules)
        {
            vkDestroyShaderModule(VkCtx.Device, module.Handle, nullptr);
        }
        
        return pipeline;
    }

    std::vector<VkFramebuffer> cl_create_vk_color_depth_framebuffers(ClVkContext &VkCtx, ClVkRenderPass &RenderPass, VkImageView &DepthView)
    {
        std::vector<VkFramebuffer> framebuffers;

        for(size_t i = 0; i < VkCtx.Swapchain.ImageViews.size(); i++)
        {
            std::array<VkImageView, 2> attachments = {
                VkCtx.Swapchain.ImageViews[i],
                DepthView
            };

            VkFramebufferCreateInfo create_info =
            {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .renderPass = RenderPass.Handle,
                .attachmentCount = static_cast<u32>(attachments.size()),
                .pAttachments = attachments.data(),
                .width = VkCtx.Swapchain.Width,
                .height = VkCtx.Swapchain.Height,
                .layers = 1
            };

            VkFramebuffer framebuffer;
            VK_CHECK(vkCreateFramebuffer(VkCtx.Device, &create_info, nullptr, &framebuffer));
            framebuffers.push_back(framebuffer);
        }
        return framebuffers;
    }

    std::vector<VkFramebuffer> cl_create_vk_color_only_framebuffers(ClVkContext &VkCtx, ClVkRenderPass &RenderPass)
    {
        std::vector<VkFramebuffer> framebuffers;

        for(size_t i = 0; i < VkCtx.Swapchain.ImageViews.size(); i++)
        {
            std::array<VkImageView, 1> attachments = {
                VkCtx.Swapchain.ImageViews[i]
            };

            VkFramebufferCreateInfo create_info =
            {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .renderPass = RenderPass.Handle,
                .attachmentCount = static_cast<u32>(attachments.size()),
                .pAttachments = attachments.data(),
                .width = VkCtx.Swapchain.Width,
                .height = VkCtx.Swapchain.Height,
                .layers = 1
            };

            VkFramebuffer framebuffer;
            VK_CHECK(vkCreateFramebuffer(VkCtx.Device, &create_info, nullptr, &framebuffer));
            framebuffers.push_back(framebuffer);
        }
        return framebuffers;
    }

    void cl_destroy_vk_framebuffers(VkDevice Device, std::vector<VkFramebuffer>& Framebuffers)
    {
        for(size_t i = 0; i < Framebuffers.size(); i++)
        {
            vkDestroyFramebuffer(Device, Framebuffers[i], nullptr);
        }
        Framebuffers.clear();
    }
    
}
