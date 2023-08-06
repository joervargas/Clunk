#include "ClVkContext.h"
#include "VkUtils.h"
#include "ClVkContext.h"

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

        vkSwapchain.Extent = { .width = Width, .height = Height };
        create_vk_swapchain(Device, Surface, QueueFamilyIndices, details.Capabilities, format, presentMode, vkSwapchain.Extent, &vkSwapchain.Handle);
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
        
        VK_CHECK(create_vk_semaphore(ctx.Device, &ctx.RenderSemaphore));
        VK_CHECK(create_vk_semaphore(ctx.Device, &ctx.WaitSemaphore));

        VK_CHECK( create_vk_command_pool(ctx.Device, ctx.Queues.Graphics.index.value(), &ctx.DrawCmds.Pool));
        VK_CHECK( allocate_vk_command_buffers(ctx.Device, ctx.DrawCmds.Pool, static_cast<u32>( ctx.Swapchain.Images.size()), ctx.DrawCmds.Buffers) );

        return ctx;
    }

    void cl_destroy_vk_context(ClVkContext* VkCtx)
    {

        cl_destroy_vk_swapchain(VkCtx->Device, &VkCtx->Swapchain);
        vkDestroyCommandPool(VkCtx->Device, VkCtx->DrawCmds.Pool, nullptr);

        vkDestroySemaphore(VkCtx->Device, VkCtx->RenderSemaphore, nullptr);
        vkDestroySemaphore(VkCtx->Device, VkCtx->WaitSemaphore, nullptr);
    
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
}
