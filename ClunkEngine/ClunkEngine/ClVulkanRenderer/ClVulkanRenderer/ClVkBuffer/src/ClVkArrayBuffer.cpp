#include "ClVkArrayBuffer.h"


#include <ClVkContext/ClVkDevice.h>

namespace Clunk
{
    void ClVkArrayBuffer::Destroy()
    {
        ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
        Allocator->DestroyBuffer(buffer, allocation);
    }

    void ClVkArrayBuffer::CopyBuffer(
            const VkCommandPool &CmdPool,
            const VkQueue &Queue,
            VkBuffer &SrcBuffer,
            VkBuffer &DstBuffer,
            VkDeviceSize BufferSize
        )
    {
        VkCommandBuffer cmdBuffer = ClVkCmds::BeginSingleTimeCmd(CmdPool);

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = BufferSize;
        vkCmdCopyBuffer(cmdBuffer, SrcBuffer, DstBuffer, 1, &copyRegion);

        ClVkCmds::EndSingleTimeCmd(CmdPool, cmdBuffer, Queue);
    }

}
