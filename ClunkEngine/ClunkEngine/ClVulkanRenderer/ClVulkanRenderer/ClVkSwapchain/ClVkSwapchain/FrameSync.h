#pragma once

#include <vulkan/vulkan.h>

#include <Defines/Defines.h>
#include <PCH/pch.h>

#include <Core/Logger.h>
#include <Core/Exception.h>

#include <ClVkContext/ClVkDevice.h>

namespace Clunk
{

    class ClVkFrameSync
    {
    public:

        std::vector<VkSemaphore> ImageAvailableSemaphores;
        std::vector<VkSemaphore> RenderFinishedSemaphores;
        std::vector<VkFence> InFlightFences;
        u32 Count;

        b8 IsInitialized() { return bIsInitialized; }

        void Create(const u32 FramesInFlightCount = 2);

        void Destroy();

    private:

        b8 bIsInitialized{false};

    };

    ;
}