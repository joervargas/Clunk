#pragma once

#include <vulkan/vulkan.h>
#include <ClVkContext/ClVkDevice.h>

#include <Core/Exception.h>
// #include <Core/Logger.h>

namespace Clunk
{

    class ClVkCmds
    {
    public:

        static VkCommandBuffer BeginSingleTimeCmd(const VkCommandPool& CmdPool)
        {
            ClVkDevice* Device = ClVkDevice::Get();
            if(!Device) { THROW_EXCEPTION("ClVkDevice* is null!"); }

            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = CmdPool;
            allocInfo.commandBufferCount = 1;

            VkCommandBuffer cmdBuffer;
            VK_CHECK(Device->AllocateCmdBuffers(&allocInfo, &cmdBuffer));

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

            return cmdBuffer;
        }

        static void EndSingleTimeCmd(
                const VkCommandPool& CmdPool, 
                const VkCommandBuffer& CmdBuffer, 
                const VkQueue& QueueFamily
            )
        {
            ClVkDevice* Device = ClVkDevice::Get();
            if(!Device) { THROW_EXCEPTION("ClVkDevice* is null!"); }

            VK_CHECK(vkEndCommandBuffer(CmdBuffer));

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &CmdBuffer;

            vkQueueSubmit(QueueFamily, 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(QueueFamily);

            Device->FreeCmdBuffers(CmdPool, 1, &CmdBuffer);
        }

        // static void BeginCmdRecord(const VkCommandBuffer& CmdBuffer, VkCommandBufferUsageFlagBits UsageFlags)
        // {
        //     VkCommandBufferBeginInfo beginInfo{};
        //     beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        //     beginInfo.flags = UsageFlags;

        //     VK_CHECK(vkBeginCommandBuffer(CmdBuffer, &beginInfo))
        // }

        // static void EndCmdRecord(const VkCommandBuffer& CmdBuffer)
        // {
        //     VK_CHECK(vkEndCommandBuffer(CmdBuffer));
        // }

        static std::vector<VkCommandBuffer> AllocateMultiCommands(const VkCommandPool& CmdPool, u32 CmdBufferCount)
        {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = CmdPool;
            allocInfo.commandBufferCount = CmdBufferCount;

            std::vector<VkCommandBuffer> cmdBuffers(CmdBufferCount);
            ClVkDevice* Device = ClVkDevice::Get();
            VK_CHECK(Device->AllocateCmdBuffers(&allocInfo, cmdBuffers.data()));

            return cmdBuffers;
        }
    };

}