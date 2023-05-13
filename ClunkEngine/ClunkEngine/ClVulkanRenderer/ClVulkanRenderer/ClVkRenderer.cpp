#include "ClVkRenderer.h"


namespace Clunk
{

    void ClVkRenderer::Init()
    {

        m_vkCtx = ClVkContext::Get();
        m_vkCtx->Create();
        m_swapchain = ClVkSwapchain::Get();
        m_swapchain->Create(MAX_FRAMES_IN_FLIGHT);

        m_descSetLayout.Init();
        m_descSetLayout.Create();
    
        // m_depthImage.Create(*m_swapchain->GetExtent());
        m_renderPass.Create(*m_swapchain->GetFormat(), m_swapchain->GetMsaaSamples(), m_swapchain->depthImage.GetFormat());

        VkPushConstantRange pushConstRange{};
        pushConstRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pushConstRange.offset = 0;
        pushConstRange.size = sizeof(Transform3d);

        m_pipelineLayout.Create(m_descSetLayout.Handle(), &pushConstRange);
        m_basicPipeline.Create(*m_renderPass.Handle(), m_swapchain->GetMsaaSamples(), m_pipelineLayout.Handle());
        // m_basicPipeline.Create(*m_renderPass.Handle(), m_swapchain->GetMsaaSamples(), m_descSetLayout.Handle());

        m_swapchain->CreateFrameBuffers(*m_renderPass.Handle());

        m_cmdPools = ClVkCmdPools::Get();
        m_cmdPools->CreatePools();
        
        m_drawCmdBuffers = ClVkCmds::AllocateMultiCommands(*m_cmdPools->GetGraphicsPool(), MAX_FRAMES_IN_FLIGHT);

        // m_VAO.Create(*m_cmdPools->GetGraphicsPool(), *m_vkCtx->QueueFamilies->Graphics(), VERTICES, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        // m_IBO.Create(*m_cmdPools->GetGraphicsPool(), *m_vkCtx->QueueFamilies->Graphics(), INDICES, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        // m_Texture.FromFile("../Assets/Images/statue.jpg");
        m_Mesh = ClVkMesh("./Assets/Models/viking_room/viking_room.obj", "./Assets/Models/viking_room/viking_room.png");
        // m_worldTransform.model = glm::mat4(1.0f);
        m_worldTransform.model = Mat4::Identity();
        // m_worldTransform.view = glm::lookAt(
        //     glm::vec3(0.0f, 2.0f, 5.0f),
        //     glm::vec3(0.0f, -1.0f, 0.0f),
        //     glm::vec3(0.0f, 1.0f, 0.0f)
        // );
        m_worldTransform.view = Mat4::LookAtRH(Vec3(0.0f, 4.0f, 0.5f), Vec3(0.0f, -1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
        // m_worldTransform.proj = glm::perspective(
        //     glm::radians(45.0f),
        //     (float)m_swapchain->GetExtent()->width / (float)m_swapchain->GetExtent()->height,
        //     0.1f,
        //     100.0f
        // );
        // m_worldTransform.proj = glm::ortho()
        // m_worldTransform.proj[1][1] *= -1;
        m_worldTransform.proj = Mat4::PerspectiveVK(45.0f, (f32)m_swapchain->GetExtent()->width / (float)m_swapchain->GetExtent()->height, 0.1f, 100.f);

        m_worldTransformUBOs.Create(m_worldTransform, MAX_FRAMES_IN_FLIGHT);
        
        // m_descPool.Create();
        // m_descSets.Create(*m_descSetLayout.Handle(), *m_descPool.Handle(), m_worldTransformUBOs.buffers, *m_Mesh.Texture()->GetView(), *m_Mesh.Texture()->GetSampler());
    }

    // void ClVkRenderer::Update()
    // {
    //     // static variable persists
    //     static auto startTime = std::chrono::high_resolution_clock::now();
    //     auto currentTime = std::chrono::high_resolution_clock::now();

    //     f32 time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    //     // m_worldTransform.model = glm::rotate(m_worldTransform.model, (time - m_lastTime) * glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //     // m_worldTransform.model = Mat4::Rotate(m_worldTransform.model, (time - m_lastTime) * Math::DegToRad(30.f), Vec3(0.0f, 0.0f, 1.0f));
    //     // m_worldTransform.model = Mat4::RotateZ(m_worldTransform.model, (time - m_lastTime) * Math::DegToRad(30.0f));
    //     // m_worldTransform.model.RotateZ((time - m_lastTime) * Math::DegToRad(30.f));
    //     m_Mesh.Update(time);
    //     m_worldTransformUBOs.Update(m_currentFrame, m_worldTransform);

    //     m_lastTime = time;
    // }

    void ClVkRenderer::Destroy()
    {
        m_vkCtx->Device->WaitIdle();

        m_worldTransformUBOs.Destroy();
        
        // m_Texture.Destroy();
        // m_IBO.Destroy();
        // m_VAO.Destroy();
        m_Mesh.Unload();

        m_swapchain->Destroy();

        // m_descPool.Destroy();
        m_descSetLayout.Destroy();

        m_renderPass.Destroy();
        m_basicPipeline.Destroy();
        m_pipelineLayout.Destroy();

        m_cmdPools->DestroyPools();
        m_vkCtx->Destroy();
    }

    void ClVkRenderer::ResetFrame()
    {

    }

    void ClVkRenderer::BeginFrame()
    {
        VK_CHECK(m_swapchain->WaitForInFlightFence(UINT64_MAX, m_currentFrame));

        // u32 imageIndex;
        VkResult result = m_swapchain->AquireNextImage(UINT64_MAX, m_currentFrame, &m_imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            CLOG_WARN("vkAquireNextImageKHR: VK_ERROR_OUT_OF_DATE_KHR.");
            m_swapchain->Reset();
            return;
        } 
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            THROW_EXCEPTION("Failed to acquire swap chain image.");
        }

        VK_CHECK(m_swapchain->ResetInFlightFence(m_currentFrame));
        VK_CHECK(vkResetCommandBuffer(m_drawCmdBuffers[m_currentFrame], 0));

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;

        VK_CHECK(vkBeginCommandBuffer(m_drawCmdBuffers[m_currentFrame], &beginInfo))

        VkExtent2D swapchainExtent = *m_swapchain->GetExtent();
        m_renderPass.Begin(m_drawCmdBuffers[m_currentFrame], m_swapchain->GetFrameBuffer(m_imageIndex), swapchainExtent);


        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapchainExtent.width);
        viewport.height = static_cast<float>(swapchainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_drawCmdBuffers[m_currentFrame], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapchainExtent;
        vkCmdSetScissor(m_drawCmdBuffers[m_currentFrame], 0, 1, &scissor);
    }

    void ClVkRenderer::RenderFrame()
    {
        // TODO: Move drawing done in Record Draw buffers to Scene ECS
        RecordDrawCmdBuffer(m_drawCmdBuffers[m_currentFrame], m_imageIndex);

        m_renderPass.End(m_drawCmdBuffers[m_currentFrame]);
        VK_CHECK(vkEndCommandBuffer(m_drawCmdBuffers[m_currentFrame]));

        VK_CHECK(m_swapchain->SubmitFrame(m_drawCmdBuffers[m_currentFrame], m_currentFrame));

        VkResult result = m_swapchain->PresentFrame(m_currentFrame, m_imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_bIsResized)
        {
            CLOG_WARN("vkQueuePresentKHR: VK_ERROR_OUT_OF_DATE_KHR or VK_SUBOPTIMAL_KHR.");
            m_bIsResized = false;
            m_swapchain->Reset();
        } 
        else if (result != VK_SUCCESS) 
        {
            THROW_EXCEPTION("Failed to present swap chain image.");
        }

        m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void ClVkRenderer::RecordDrawCmdBuffer(const VkCommandBuffer &DrawBuffer, const u32 &ImageIndex)
    {
        // update  properties
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();

        f32 time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        // m_worldTransform.model = glm::rotate(m_worldTransform.model, (time - m_lastTime) * glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        // m_worldTransform.model = Mat4::Rotate(m_worldTransform.model, (time - m_lastTime) * Math::DegToRad(30.f), Vec3(0.0f, 0.0f, 1.0f));
        // m_worldTransform.model = Mat4::RotateZ(m_worldTransform.model, (time - m_lastTime) * Math::DegToRad(30.0f));
        // m_worldTransform.model.RotateZ((time - m_lastTime) * Math::DegToRad(30.f));
        m_Mesh.Update(time);
        m_worldTransformUBOs.Update(m_currentFrame, m_worldTransform);

        m_lastTime = time;

        // render
        vkCmdBindPipeline(DrawBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_basicPipeline.Get());

        // m_Mesh.Render(DrawBuffer, *m_pipelineLayout.Handle(), m_descSets.Get(m_currentFrame));
        m_Mesh.Render(DrawBuffer, *m_pipelineLayout.Handle(), m_worldTransformUBOs.buffers[m_currentFrame]);
    }
}