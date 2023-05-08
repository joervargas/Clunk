#pragma once

#include <vulkan/vulkan.h>

#include <PCH/pch.h>

namespace Clunk
{
    class ClVkCmdPools
    {
    public:

        b8 bGraphicsPoolActive{false};
        b8 bComputePoolActive{false};
        b8 bTransferPoolActive{false};

        void CreateGraphicsPool();
        void DestroyGraphicsPool();

        void CreateComputePool();
        void DestroyComputePool();

        // void CreateTransferPool();
        // void DestroyTransferPool();

        void CreatePools();
        void DestroyPools();

        static ClVkCmdPools* Get() { return &m_ClVkCmdPools; }

        const VkCommandPool* GetGraphicsPool() { return bGraphicsPoolActive? &m_graphicsPool : nullptr; }
        const VkCommandPool* GetComputePool() { return bComputePoolActive? &m_computePool : nullptr; }
        // const VkCommandPool* GetTransferPool() { return bTransferPoolActive? &m_transferPool : nullptr; }

    private:

        ClVkCmdPools() : bGraphicsPoolActive(false), bComputePoolActive(false) {};
        ~ClVkCmdPools() {};

        static ClVkCmdPools m_ClVkCmdPools;

        VkCommandPool m_graphicsPool;
        VkCommandPool m_computePool;
        // VkCommandPool m_transferPool;
    };
}