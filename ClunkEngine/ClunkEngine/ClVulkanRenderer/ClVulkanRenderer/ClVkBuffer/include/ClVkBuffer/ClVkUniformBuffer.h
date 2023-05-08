#pragma once

#include <vulkan/vulkan.h>
#include <VkMemAllocator/vk_mem_alloc.h>

#include <ClVkContext/ClVkMemAllocator.h>

#include <PCH/pch.h>

#include <Core/Exception.h>

#include <memory>

namespace Clunk
{
    template <typename T>
    class ClVkUniformBuffer
    {
    public:

        VkBuffer buffer;
        VmaAllocation allocation;

        ClVkUniformBuffer() {};
        ~ClVkUniformBuffer() {};

        void Create(T& Data)
        {
            VkDeviceSize dataSize = static_cast<VkDeviceSize>(sizeof(Data));

            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = dataSize;
            bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                VMA_ALLOCATION_CREATE_MAPPED_BIT;

            ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
            VK_CHECK(Allocator->CreateBuffer(&bufferInfo, &allocInfo, &buffer, &allocation, nullptr));

            Update(Data);
        }


        void Destroy()
        {
            ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
            Allocator->DestroyBuffer(buffer, allocation);
        }


        void Update(T& NewData)
        {
            ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
            // Copy NewData to memory
            void* pData;
            // Map pData to allocated memory
            Allocator->MapMemory(allocation, &pData);
            // Copy values to pData
                memcpy(pData, &NewData, sizeof(T));
            // Unmap memory
            Allocator->UnmapMemory(allocation);
        }

    private:

    };

template <typename T>
    class ClVkUniformBuffers
    {
    public:

        std::vector<VkBuffer> buffers;
        std::vector<VmaAllocation> allocations;

        ClVkUniformBuffers() {};
        ~ClVkUniformBuffers() {};

        void Create(T& Data, size_t Size)
        {
            VkDeviceSize dataSize = static_cast<VkDeviceSize>(sizeof(Data));

            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = dataSize;
            bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                VMA_ALLOCATION_CREATE_MAPPED_BIT;

            buffers.resize(Size); allocations.resize(Size);
            for(size_t i = 0; i < Size; i++)
            {
                ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
                VK_CHECK(Allocator->CreateBuffer(&bufferInfo, &allocInfo, &buffers[i], &allocations[i], nullptr));
                Update(i, Data);
            }
        }


        void Destroy()
        {
            ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
            for(size_t i = 0; i < allocations.size(); i++)
            {
                Allocator->DestroyBuffer(buffers[i], allocations[i]);
            }
        }


        void Update(size_t Index,T& NewData)
        {
            ClVkMemAllocator* Allocator = ClVkMemAllocator::Get();
            // Copy NewData to memory
            void* pData;
            // Map pData to allocated memory
            Allocator->MapMemory(allocations[Index], &pData);
            // Copy values to pData
                memcpy(pData, &NewData, sizeof(T));
            // Unmap memory
            Allocator->UnmapMemory(allocations[Index]);
        }

    private:

    };
}