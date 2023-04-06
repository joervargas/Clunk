#pragma once

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <ClVkContext/ClVkDevice.h>
#include <ClVkContext/ClVkPhysicalDevice.h>

#include <Defines/Defines.h>
#include <Core/Exception.h>
#include <Core/Logger.h>

namespace Clunk
{
    class ClVkSampler
    {
    public:

        static void Create(u32 MipLevels, VkSampleCountFlagBits MsaaSamples, VkSampler* pSampler)
        {
            VkSamplerCreateInfo samplerInfo{};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;
            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

            samplerInfo.anisotropyEnable = VK_TRUE;
            samplerInfo.maxAnisotropy = MsaaSamples;
            // ClVkPhysicalDevice* PhysDevice = ClVkPhysicalDevice::Get();
            // samplerInfo.maxAnisotropy = PhysDevice->Properties()->limits.maxSamplerAnisotropy;
            
            samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_FALSE;
            samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerInfo.mipLodBias = 0.0f;
            samplerInfo.minLod = 0.0f;
            samplerInfo.maxLod = static_cast<float>(MipLevels);

            ClVkDevice* Device = ClVkDevice::Get();
            VK_CHECK(Device->CreateSampler(&samplerInfo, nullptr, pSampler));
        }
    };
}