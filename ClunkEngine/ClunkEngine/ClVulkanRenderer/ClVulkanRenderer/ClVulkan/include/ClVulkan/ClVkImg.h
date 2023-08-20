#pragma once

#include <PCH/pch.h>
#include "VkDefines.h"
#include <vulkan/vulkan.h>

// Forward Declarations
struct ClVkContext;

namespace Clunk::Vk
{

    /**
     * @brief Create VkImage and VkDeviceMemory handles and 
     * 
     * @param Device VkDevice handle
     * @param PhysicalDevice VkPhysicalDevice handle
     * @param Width Image width
     * @param Height Image height
     * @param Format Image format
     * @param Tiling VkImageTiling enum
     * @param Usage VkImageUsage flags enum
     * @param Properties VkMemoryProperties
     * @param Image VkImage* handle to assign to
     * @param Memory VkDeviceMemory* handle to assign to
     * @param Flags VkImageCreateFlags. Default = 0
     * @param MipLevels u32 Mip level count. Default = 0
     */
    void create_vk_image(
        VkDevice Device, VkPhysicalDevice PhysicalDevice, 
        u32 Width, u32 Height, VkFormat Format, VkImageTiling Tiling, 
        VkImageUsageFlags Usage, VkMemoryPropertyFlags Properties, 
        VkImage* ppImage, VkDeviceMemory* pMemory, 
        VkImageCreateFlags Flags = 0, u32 MipLevels = 1
    );

    /**
     * @brief Create a VkImageView object
     * 
     * @param Device Device handle
     * @param Image VkImage
     * @param Format VkFormat of the image
     * @param AspectFlags 
     * @param pImageView VkImageView* to create
     * @param ImageViewType VkImageViewType
     * @param LayerCount u32 Image layer count
     * @param MipLevels u32 Image mip level count
     */
    void create_vk_image_view(
        VkDevice Device, 
        VkImage Image, VkFormat Format, 
        VkImageAspectFlags AspectFlags, 
        VkImageView* pImageView, 
        VkImageViewType ImageViewType = VK_IMAGE_VIEW_TYPE_2D, 
        u32 LayerCount = 1, u32 MipLevels = 1
    );

    /**
     * @brief Create a VkSampler handle
     * 
     * @param Device VkDevice handle
     * @param pSampler VkSampler* handle to assign to
     */
    void create_vk_sampler(VkDevice Device, VkSampler* pSampler);

    /**
     * @brief Searches a list of VkFormat Candidates (std::vector<VkFormat>) and returns the supported format.
     * 
     * @param PhysicalDevice VkPhysicalDevice handle
     * @param Candidates std::vector<VkFormat> list of VkFormat candidates to search through.
     * @param Tiling VkImageTiling - Desired Tiling feature
     * @param Features VkFormatFeatureFlags - Desired format features
     * @return VkFormat 
     */
    VkFormat find_supported_vk_format(
        VkPhysicalDevice PhysicalDevice,
        const std::vector<VkFormat>& Candidates,
        VkImageTiling Tiling,
        VkFormatFeatureFlags Features
    );

    /**
     * @brief Transitions VkImageLayout from an old layout to a new desired layout.
     * 
     * @param CmdBuffer 
     * @param Image 
     * @param Format 
     * @param OldLayout 
     * @param NewLayout 
     * @param LayerCount 
     * @param MipLevels 
     */
    void transition_vk_image_layout(
        VkCommandBuffer CmdBuffer, 
        VkImage Image, VkFormat Format, 
        VkImageLayout OldLayout, VkImageLayout NewLayout, 
        u32 LayerCount = 1, u32 MipLevels =1
    );

    /**
     * @brief Copies a VkBuffer to a VkImage
     * 
     * @param CmdBuffer VkCommandBuffer to execute the operation
     * @param Buffer VkBuffer to be copied
     * @param Image VkImage to be copied to
     * @param Width Image's width
     * @param Height Image's height
     * @param LayerCount Number of layers in the image
     */
    void copy_vk_cmd_buffer_to_image(
        VkCommandBuffer CmdBuffer, 
        VkBuffer Buffer, VkImage Image, 
        u32 Width, u32 Height, u32 
        LayerCount = 1
    );

    /**
     * @brief Finds a supported depth image format. (Uses fn 'find_supported_vk_format' intrinsically.)
     * 
     * @param PhysicalDevice VkPhysicalDevice handle
     * @return VkFormat 
     */
    VkFormat find_vk_format_depth_img(VkPhysicalDevice PhysicalDevice);

    /**
     * @brief Returns true if VkFormat supports stencil components
     * 
     * @param Format 
     * @return bool
     */
    bool has_vk_stencil_component(VkFormat Format);

    struct ClVkImage final
    {
        VkImage Handle = nullptr;
        VkDeviceMemory Memory= nullptr;
        VkImageView View = nullptr;
    };

    /**
     * @brief Creates and returns a ClVkImage Struct
     * 
     * @param VkCtx ClVkContext
     * @param Filename const char* file name with path
     * @return ClVkImage 
     */
    ClVkImage cl_create_vk_image(ClVkContext& VkCtx, const char* Filename);

    /**
     * @brief Creates a DepthImage ClVkImage
     * 
     * @param VkCtx ClVkContext struct
     * @param Width u32 image width
     * @param Height u32 image height
     * @return ClVkImage 
     */
    ClVkImage cl_create_vk_depth_image(ClVkContext& VkCtx, u32 Width, u32 Height);

    /**
     * @brief Destroys ClVkImage and all its resources
     * 
     * @param Device VkDevice handle
     * @param pImage* ClVkImage pointer handle to destroy
     */
    void cl_destroy_vk_image(const VkDevice Device, ClVkImage* pImage);
    
    struct ClVkTexture final
    {
        u32 Width;
        u32 Height;
        u32 Depth;
        VkFormat Format;

        ClVkImage Image;
        VkSampler Sampler = nullptr;

        // Offscreen buffers require VK_IMAGE_LAYOUT_GENERAL && static features have VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        VkImageLayout desiredLayout;
    };

}
