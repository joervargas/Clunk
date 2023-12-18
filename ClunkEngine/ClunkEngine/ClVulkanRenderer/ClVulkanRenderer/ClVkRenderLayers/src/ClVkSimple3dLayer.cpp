#include "ClVkRenderLayers/ClVkSimple3dLayer.h"
#include "ClVkSimple3dLayer.h"


namespace Clunk::Vk
{
    ClVkSimple3dLayer::ClVkSimple3dLayer(ClVkContext &VkCtx, const ClVkImage& DepthImage, const char* MeshFile, const char* TextureFile) :
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


    }

    ClVkSimple3dLayer::~ClVkSimple3dLayer()
    {
    }

    void ClVkSimple3dLayer::Destroy(ClVkContext &VkCtx)
    {
    }

    void ClVkSimple3dLayer::Update(u32 CurrentIndex, ClVkBuffer &TransformUniform, ClVkImage &DepthImage, f32 DeltaTime)
    {
    }

    void ClVkSimple3dLayer::DrawFrame(const ClVkContext &VkCtx, const VkCommandBuffer &CmdBuffer, size_t CurrentImage)
    {
    }

    void ClVkSimple3dLayer::CreateDescriptor(ClVkContext &VkCtx)
    {
    }

    void ClVkSimple3dLayer::CreatePipeline(const ClVkContext &VkCtx, std::vector<ClVkShaderModule> &ShaderModules, ClVkRenderPass &RenderPass, VkPipelineLayout &Layout, VkExtent2D CustomExtent)
    {
    }
    
    void ClVkSimple3dLayer::Draw(const ClVkContext &VkCtx, VkCommandBuffer CmdBuffer)
    {
    }
}