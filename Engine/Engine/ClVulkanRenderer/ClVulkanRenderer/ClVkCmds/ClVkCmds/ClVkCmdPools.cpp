#include "ClVkCmdPools.h"
#include <ClVkContext/ClVkDevice.h>
#include <ClVkContext/ClVkQueueFamilies.h>

#include <Core/Logger.h>
#include <Core/Exception.h>

namespace Clunk
{
    ClVkCmdPools ClVkCmdPools::m_ClVkCmdPools;

    void ClVkCmdPools::CreateGraphicsPool()
    {
        if(bGraphicsPoolActive) { return; }
        CLOG_INFO("Creating Vk Graphics CmdPool...")
        ClVkQueueFamilies* QueueFamilies = ClVkQueueFamilies::Get();

        VkCommandPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.queueFamilyIndex = QueueFamilies->Indices()->graphics.value();
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        ClVkDevice* Device = ClVkDevice::Get();
        VK_CHECK(Device->CreateCmdPool(&createInfo, nullptr, &m_graphicsPool));

        bGraphicsPoolActive = true;
        CLOG_INFO("Vk Grpahics CmdPool Created.");
    }

    void ClVkCmdPools::DestroyGraphicsPool()
    {
        if(!bGraphicsPoolActive) { return; }

        CLOG_INFO("Destroying Vk Graphics CmdPool...");

        ClVkDevice* Device = ClVkDevice::Get();
        Device->DestroyCmdPool(m_graphicsPool, nullptr);
        bGraphicsPoolActive = false;

        CLOG_INFO("Vk Graphics CmdPool Destroyed.");
    }

    void ClVkCmdPools::CreateComputePool()
    {
        if(bComputePoolActive) { return; }
        CLOG_INFO("Creating Vk Compute CmdPool...")
        ClVkQueueFamilies* QueueFamilies = ClVkQueueFamilies::Get();

        VkCommandPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.queueFamilyIndex = QueueFamilies->Indices()->compute.value();
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        ClVkDevice* Device = ClVkDevice::Get();
        VK_CHECK(Device->CreateCmdPool(&createInfo, nullptr, &m_computePool));

        bComputePoolActive = true;
        CLOG_INFO("Vk Compute CmdPool Created.");
    }

    void ClVkCmdPools::DestroyComputePool()
    {
        if(!bComputePoolActive) { return; }

        CLOG_INFO("Destroying Vk Compute CmdPool...");
        
        ClVkDevice* Device = ClVkDevice::Get();
        Device->DestroyCmdPool(m_computePool, nullptr);
        bComputePoolActive = false;

        CLOG_INFO("Vk Compute CmdPool Destroyed.");
    }

    // void ClVkCmdPools::CreateTransferPool()
    // {
    //     if(bTransferPoolActive) { return; }
    //     CLOG_INFO("Creating Vk Transfer CmdPool...")
    //     ClVkQueueFamilies* QueueFamilies = ClVkQueueFamilies::GetClVkQueueFamilies();

    //     VkCommandPoolCreateInfo createInfo{};
    //     createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    //     createInfo.queueFamilyIndex = QueueFamilies->Indices()->transfer.value();
    //     createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    //     ClVkDevice* Device = ClVkDevice::Get();
    //     VK_CHECK(Device->CreateCmdPool(&createInfo, nullptr, &m_transferPool));

    //     bTransferPoolActive = true;
    //     CLOG_INFO("Vk Transfer CmdPool Created.");
    // }

    // void ClVkCmdPools::DestroyTransferPool()
    // {
    //     if(!bTransferPoolActive) { return; }

    //     CLOG_INFO("Destroying Vk Transfer CmdPool...");

    //     ClVkDevice* Device = ClVkDevice::Get();
    //     Device->DestroyCmdPool(m_transferPool, nullptr);
    //     bTransferPoolActive = false;

    //     CLOG_INFO("Vk Transfer CmdPool Destroyed.");
    // }

    void ClVkCmdPools::CreatePools()
    {
        CreateGraphicsPool();
        CreateComputePool();
        // CreateTransferPool();
    }

    void ClVkCmdPools::DestroyPools()
    {
        DestroyGraphicsPool();
        DestroyComputePool();
        // DestroyTransferPool();
    }
}
