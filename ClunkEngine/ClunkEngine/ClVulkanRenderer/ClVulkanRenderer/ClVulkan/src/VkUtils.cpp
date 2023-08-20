#include "VkUtils.h"
#include "ClVkImg.h"

namespace Clunk::Vk
{
    
    bool is_vk_physical_device_suitable(VkSurfaceKHR Surface, VkPhysicalDevice PhysicalDevice)
    {
        // Query for properties
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(PhysicalDevice, &deviceProperties);

        VkPhysicalDeviceFeatures2 deviceFeatures{};
        deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        vkGetPhysicalDeviceFeatures2(PhysicalDevice, &deviceFeatures);

        const b8 isDiscreteGPU = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        const b8 isIntegratedGPU = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
        const b8 isGPU = isDiscreteGPU || isIntegratedGPU;

        VkBool32 present_supported = false;
        std::optional<u32> graphics_index = find_vk_queue_family_index(PhysicalDevice, VK_QUEUE_GRAPHICS_BIT);
        if(graphics_index.has_value())
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, graphics_index.value(), Surface, &present_supported);
        } else {
            present_supported = false;
        }

        return isGPU && deviceFeatures.features.geometryShader && present_supported;
    }

    VkResult find_suitable_vk_physical_device(const VkInstance Instance, VkSurfaceKHR Surface, VkPhysicalDevice *pPhysicalDevice)
    {
        CLOG_INFO("Finding Suitable Physical Device...");

        u32 deviceCount = 0;
        VK_CHECK(vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr));
        if(!deviceCount) { return VK_ERROR_INITIALIZATION_FAILED; } // No device to loop through

        std::vector<VkPhysicalDevice> devices(deviceCount);
        VK_CHECK(vkEnumeratePhysicalDevices(Instance, &deviceCount, devices.data()));

        for(const auto& device: devices)
        {
            if(is_vk_physical_device_suitable(Surface, device))
            {
                *pPhysicalDevice = device;

                CLOG_INFO("Physical Device found!");
                return VK_SUCCESS;
            }
        }

        CLOG_ERROR("Physical Device not found!");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    std::optional<u32> find_vk_queue_family_index(VkPhysicalDevice PhysicalDevice, VkQueueFlags DesiredQueueFlags)
    {
        // Determine number of Queue families on a device. 
        u32 familyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &familyCount, nullptr);
        // Vector of VkQueueFamilyProperties with the size of the familyCount
        std::vector<VkQueueFamilyProperties> families(familyCount);

        // Populate the above vector
        vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &familyCount, families.data());
        std::optional<u32> result = std::nullopt;
        for(u32 i = 0; i != families.size(); i++)
        {
            if(families[i].queueCount && families[i].queueFlags & DesiredQueueFlags)
            {
                result = i;
                return result;
            }
        }

        return result;
    }

    void create_vk_device(VkPhysicalDevice PhysicalDevice, std::vector<u32> QueueFamilyIndices, VkDevice *pDevice)
    {
        CLOG_INFO("Creating VkDevice handle...");

        const f32 queuePriority = 1.0f;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(QueueFamilyIndices.size());
        for(u32 i = 0; i < QueueFamilyIndices.size(); i++)
        {
            const VkDeviceQueueCreateInfo queueCI =
            {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueFamilyIndex = QueueFamilyIndices[i],
                .queueCount = 1,
                .pQueuePriorities = &queuePriority,
            };
            queueCreateInfos.push_back(queueCI);
        }

        const std::vector<const char*> device_exts =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        const VkDeviceCreateInfo deviceCI =
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size()),
            .pQueueCreateInfos = queueCreateInfos.data(),
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = static_cast<u32>(device_exts.size()),
            .ppEnabledExtensionNames = device_exts.data()
        };

        VK_CHECK( vkCreateDevice(PhysicalDevice, &deviceCI, nullptr, pDevice) );
        
        CLOG_INFO("VkDevice handle created.")
    }

    void create_vk_mem_allocator(const VkInstance Instance, u32 Vk_API_Version, const VkPhysicalDevice PhysicalDevice, const VkDevice Device, VmaAllocator *pMemAllocator)
    {
        CLOG_INFO("Creating VmaAllocator...");

        VmaAllocatorCreateInfo ci =
        {
            .physicalDevice = PhysicalDevice,
            .device = Device,
            .instance = Instance,
            .vulkanApiVersion = Vk_API_Version
        };
        VK_CHECK(vmaCreateAllocator(&ci, pMemAllocator));

        CLOG_INFO("VmaAllocator created")
    }

    VkSwapchainDetails query_vk_swapchain_details(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface)
    {
        VkSwapchainDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, Surface, &details.Capabilities);

        u32 formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &formatCount, nullptr);
        if(formatCount)
        {
            details.Formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &formatCount, details.Formats.data());
        }

        u32 presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &presentModeCount, nullptr);
        if(presentModeCount)
        {
            details.PresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &presentModeCount, details.PresentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR choose_vk_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& AvailableFormats)
    {
        for(const VkSurfaceFormatKHR& availableFormat : AvailableFormats)
        {
            if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }
        return AvailableFormats[0];
    }

    VkPresentModeKHR choose_vk_swap_present_mode(const std::vector<VkPresentModeKHR>& AvailablePresentModes)
    {
        for(const VkPresentModeKHR availablePresentMode : AvailablePresentModes)
        {
            if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) { return availablePresentMode; }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    u32 choose_vk_swapchain_image_count(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        const u32 imageCount = capabilities.minImageCount + 1;

        const bool imageCountExceeded = capabilities.maxImageCount && imageCount > capabilities.maxImageCount;

        return imageCountExceeded ? capabilities.maxImageCount : imageCount;
    }

    void create_vk_swapchain(const VkDevice Device, VkSurfaceKHR Surface, std::vector<u32> QueueFamilyIndices, VkSurfaceCapabilitiesKHR Capabilities, VkSurfaceFormatKHR Format, VkPresentModeKHR PresentMode, u32 Width, u32 Height, VkSwapchainKHR *pSwapchain, bool bSupportScreenshots)
    {
        CLOG_INFO("Creating VkSwapchain...");

        const VkSwapchainCreateInfoKHR ci =
        {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .flags = 0,
            .surface = Surface,
            .minImageCount = choose_vk_swapchain_image_count(Capabilities),
            .imageFormat = Format.format,
            .imageColorSpace = Format.colorSpace,
            .imageExtent = VkExtent2D{ .width = Width, .height = Height },
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = static_cast<u32>(QueueFamilyIndices.size()),
            .pQueueFamilyIndices = QueueFamilyIndices.data(),
            .preTransform = Capabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = PresentMode,
            .clipped = VK_TRUE,
            .oldSwapchain = VK_NULL_HANDLE
        };

        VK_CHECK(vkCreateSwapchainKHR(Device, &ci, nullptr, pSwapchain));

        CLOG_INFO("VkSwapchain created");
    }

    void create_vk_swapchain_images(const VkDevice Device, const VkSwapchainKHR Swapchain, std::vector<VkImage> &SwapchainImages, std::vector<VkImageView> &SwapchainImageViews)
    {
        u32 imageCount = 0;
        VK_CHECK(vkGetSwapchainImagesKHR(Device, Swapchain, &imageCount, nullptr));

        SwapchainImages.resize(imageCount);
        SwapchainImageViews.resize(imageCount);

        VK_CHECK(vkGetSwapchainImagesKHR(Device, Swapchain, &imageCount, SwapchainImages.data()));

        for(u32 i = 0; i < imageCount; i++)
        {
            create_vk_image_view(Device, SwapchainImages[i], VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, &SwapchainImageViews[i]);
        }
    }

    VkResult create_vk_command_pool(const VkDevice Device, u32 QueueFamilyIndex, VkCommandPool *pCommandPool)
    {
        const VkCommandPoolCreateInfo ci =
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .queueFamilyIndex = QueueFamilyIndex
        };

        return vkCreateCommandPool(Device, &ci, nullptr, pCommandPool);
    }

    VkResult allocate_vk_command_buffers(const VkDevice Device, const VkCommandPool CommandPool, u32 BufferCount, std::vector<VkCommandBuffer> &CommandBuffers)
    {
        const VkCommandBufferAllocateInfo ai =
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = CommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = BufferCount
        };
        return vkAllocateCommandBuffers(Device, &ai, CommandBuffers.data());
    }


    VkResult create_vk_semaphore(VkDevice Device, VkSemaphore *pSemaphore)
    {
        const VkSemaphoreCreateInfo ci =
        {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        };

        return vkCreateSemaphore(Device, &ci, nullptr, pSemaphore);
    }

    VkResult create_vk_pipeline_layout(VkDevice Device, u32 DescriptorSetLayoutCount, VkDescriptorSetLayout *pDescriptorSetLayouts, u32 PushConstantRangeCount, VkPushConstantRange *pPushConstantRange, VkPipelineLayout *pPipeLineLayout)
    {
        const VkPipelineLayoutCreateInfo create_info =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .setLayoutCount = DescriptorSetLayoutCount,
            .pSetLayouts = pDescriptorSetLayouts,
            .pushConstantRangeCount = PushConstantRangeCount,
            .pPushConstantRanges = pPushConstantRange
        };
        return vkCreatePipelineLayout(Device, &create_info, nullptr, pPipeLineLayout);
    }
}

