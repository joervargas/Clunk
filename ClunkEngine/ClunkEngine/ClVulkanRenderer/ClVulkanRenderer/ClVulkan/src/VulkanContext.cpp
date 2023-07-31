#include "VulkanContext.h"
#include "VkUtils.h"

namespace Clunk::Vk
{

    void query_vulkan_queues_indices(const VkPhysicalDevice PhysicalDevice, VulkanQueues *Queues)
    {
        if (!Queues) { THROW_EXCEPTION("VulkanQueus* must not be nullptr!"); }

        Queues->Graphics.index = find_vk_queue_family_index(PhysicalDevice, VK_QUEUE_GRAPHICS_BIT);
    }

    void query_vulkan_queues_handles(const VkDevice Device, VulkanQueues *Queues)
    {
        if (!Queues) { THROW_EXCEPTION("VulkanQueus* must not be nullptr!"); }

        if(Queues->Graphics.index.has_value())
        {
            vkGetDeviceQueue(Device, Queues->Graphics.index.value(), 0, &Queues->Graphics.Handle);
        }
    }

    std::vector<u32> get_vulkan_queues_indices_list(const VulkanQueues* Queues)
    {
        std::vector<u32> result;
        if (Queues->Graphics.index.has_value())
        {
            result.push_back(Queues->Graphics.index.value());
        }
        return result;
    }

    VulkanSwapchain create_vulkan_swapchain(const VkDevice Device, VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface, std::vector<u32> QueueFamilyIndices, u32 Width, u32 Height)
    {
        VulkanSwapchain vkSwapchain;

        VkSwapchainDetails details = query_vk_swapchain_details(PhysicalDevice, Surface);
        VkSurfaceFormatKHR format = choose_vk_swap_surface_format(details.Formats);
        vkSwapchain.Format = format.format;
        VkPresentModeKHR presentMode = choose_vk_swap_present_mode(details.PresentModes);

        vkSwapchain.Extent = { .width = Width, .height = Height };
        create_vk_swapchain(Device, Surface, QueueFamilyIndices, details.Capabilities, format, presentMode, vkSwapchain.Extent, &vkSwapchain.Handle);
        create_vk_swapchain_images(Device, vkSwapchain.Handle, vkSwapchain.Images, vkSwapchain.ImageViews);
        
        return vkSwapchain;
    }

    void destroy_vulkan_swapchain(const VkDevice Device, VulkanSwapchain *pSwapchain)
    {
        for(size_t i = 0; i < pSwapchain->ImageViews.size(); i++)
        {
            vkDestroyImageView(Device, pSwapchain->ImageViews[i], nullptr);
        }
        vkDestroySwapchainKHR(Device, pSwapchain->Handle, nullptr);
        pSwapchain->Handle = nullptr;
    }

    VulkanContext create_vulkan_context(const VulkanLoader &Loader, u32 Width, u32 Height)
    {
        VulkanContext ctx;

        VK_CHECK( find_suitable_vk_physical_device(Loader.Instance, Loader.Surface, &ctx.PhysicalDevice) );

        query_vulkan_queues_indices(ctx.PhysicalDevice, &ctx.Queues);
        std::vector<u32> queue_indices = get_vulkan_queues_indices_list(&ctx.Queues);
        
        create_vk_device(ctx.PhysicalDevice, queue_indices, &ctx.Device);
        query_vulkan_queues_handles(ctx.Device, &ctx.Queues);

        create_vk_mem_allocator(Loader.Instance, Loader.Vk_API_Version, ctx.PhysicalDevice, ctx.Device, &ctx.MemAllocator );

        ctx.Swapchain = create_vulkan_swapchain(ctx.Device, ctx.PhysicalDevice, Loader.Surface, queue_indices, Width, Height);
        
        VK_CHECK(create_vk_semaphore(ctx.Device, &ctx.RenderSemaphore));
        VK_CHECK(create_vk_semaphore(ctx.Device, &ctx.WaitSemaphore));

        VK_CHECK( create_vk_command_pool(ctx.Device, ctx.Queues.Graphics.index.value(), &ctx.DrawCmds.Pool));
        VK_CHECK( allocate_vk_command_buffers(ctx.Device, ctx.DrawCmds.Pool, static_cast<u32>( ctx.Swapchain.Images.size()), ctx.DrawCmds.Buffers) );

        return ctx;
    }

    void destroy_vulkan_context(VulkanContext* Ctx)
    {

        destroy_vulkan_swapchain(Ctx->Device, &Ctx->Swapchain);
        vkDestroyCommandPool(Ctx->Device, Ctx->DrawCmds.Pool, nullptr);

        vkDestroySemaphore(Ctx->Device, Ctx->RenderSemaphore, nullptr);
        vkDestroySemaphore(Ctx->Device, Ctx->WaitSemaphore, nullptr);
    
        vmaDestroyAllocator(Ctx->MemAllocator);
        vkDestroyDevice(Ctx->Device, nullptr);
    }

}
