#include "ClVkRenderPass.h"
#include <ClVkContext/ClVkDevice.h>
#include <ClVkSwapchain/ClVkSwapchain.h>

#include <Core/Logger.h>
#include <Core/Exception.h>


namespace Clunk{
    void ClVKRenderPass::Create(const VkFormat& SwapchainFormat, const VkSampleCountFlagBits& MsaaSampleCount, const VkFormat& DepthImageFormat)
    {
        CLOG_INFO("Creating Vk RenderPass...");

        // ClVkSwapchain* Swapchain = ClVkSwapchain::Get();

        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = SwapchainFormat;
        colorAttachment.samples = MsaaSampleCount;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0; // Referenced from fragment shader: layout(location = 0)
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = DepthImageFormat;
        depthAttachment.samples = MsaaSampleCount;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachmentResolve{};
        colorAttachmentResolve.format = SwapchainFormat;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentResolverRef{};
        colorAttachmentResolverRef.attachment = 2;
        colorAttachmentResolverRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


        VkSubpassDescription subpassDesc{};
        subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDesc.colorAttachmentCount = 1;
        subpassDesc.pColorAttachments = &colorAttachmentRef;
        subpassDesc.pDepthStencilAttachment = &depthAttachmentRef;
        subpassDesc.pResolveAttachments = &colorAttachmentResolverRef;

        VkSubpassDependency dependencey{};
        dependencey.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependencey.dstSubpass = 0;
        dependencey.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependencey.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependencey.srcAccessMask = 0;
        dependencey.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 3> attachments = {
            colorAttachment,
            depthAttachment,
            colorAttachmentResolve
        };

        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.attachmentCount = static_cast<u32>(attachments.size());
        createInfo.pAttachments = attachments.data();
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &subpassDesc;
        createInfo.dependencyCount = 1;
        createInfo.pDependencies = &dependencey;

        ClVkDevice* Device = ClVkDevice::Get();
        VK_CHECK(Device->CreateRenderPass(&createInfo, nullptr, &m_render_pass));
        bIsInitialized = true;

        CLOG_INFO("Vk RenderPass Created.")
    }


    void ClVKRenderPass::Destroy()
    {
        CLOG_INFO("Destroying Vk RenderPass...");

        ClVkDevice* Device = ClVkDevice::Get();
        Device->DestroyRenderPass(m_render_pass, nullptr);
        bIsInitialized = false;

        CLOG_INFO("Vk RenderPass Destroyed.");
    }

    void ClVKRenderPass::Begin(const VkCommandBuffer &DrawBuffer, const VkFramebuffer& Framebuffer, const VkExtent2D& Extent)
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_render_pass;
        // renderPassInfo.framebuffer = Framebuffers[ImageIndex];
        renderPassInfo.framebuffer = Framebuffer;
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = Extent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<u32>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(DrawBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void ClVKRenderPass::End(const VkCommandBuffer &DrawBuffer)
    {
        vkCmdEndRenderPass(DrawBuffer);
    }
}