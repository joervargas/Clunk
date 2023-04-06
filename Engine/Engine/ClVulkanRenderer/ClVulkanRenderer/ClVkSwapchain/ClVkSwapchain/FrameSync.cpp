#include "FrameSync.h"


namespace Clunk
{

    void ClVkFrameSync::Create(const u32 FramesInFlightCount)
    {
        Count = FramesInFlightCount;
        ImageAvailableSemaphores.resize(Count);
        RenderFinishedSemaphores.resize(Count);
        InFlightFences.resize(Count);
    
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        
        ClVkDevice* Device = ClVkDevice::Get();
        for(size_t i = 0; i < Count; i++)
        {
            VK_CHECK(Device->CreateSemaphore(&semaphoreInfo, nullptr, &ImageAvailableSemaphores[i]));
            VK_CHECK(Device->CreateSemaphore(&semaphoreInfo, nullptr, &RenderFinishedSemaphores[i]));
            VK_CHECK(Device->CreateFence(&fenceInfo, nullptr, &InFlightFences[i]));
        }
        bIsInitialized = true;
    }

    void ClVkFrameSync::Destroy()
    {
        ClVkDevice* Device = ClVkDevice::Get();
        for(size_t i = 0; i < Count; i++)
        {
            Device->DestroySemaphore(ImageAvailableSemaphores[i], nullptr);
            Device->DestroySemaphore(RenderFinishedSemaphores[i], nullptr);
            Device->DestroyFence(InFlightFences[i], nullptr);
        }
        bIsInitialized = false;
    }

}