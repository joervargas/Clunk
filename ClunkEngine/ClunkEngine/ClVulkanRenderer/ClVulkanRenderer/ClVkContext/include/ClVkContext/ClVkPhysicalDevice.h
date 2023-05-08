#pragma once

// #include <ClVkLoader/ClVkLoader.h>
#include <vulkan/vulkan.h>
#include <PCH/pch.h>


class ClVkPhysicalDevice
{
public:

    void Pick();

    // void Destroy();

    /**
     * @brief Get the ClPhysDevice* from the Singleton intance.
     * @return ClVkPhysicalDevice* 
     */
    static ClVkPhysicalDevice* Get() { return &m_ClVkPhysicalDevice; }

    /**
     * @brief Returns a VkPhysicalDevice* to the currently selected Physical Device
     * @return VkPhysicalDevice* 
     */
    VkPhysicalDevice* Handle() { return &m_physicalDevice; }

    // const VkPhysicalDeviceProperties* Properties() { return &m_physicalDeviceProperties; }
    // const VkPhysicalDeviceMemoryProperties* MemoryProperties() { return &m_physicalDeviceMemoryProperties; }
    // const VkPhysicalDeviceFeatures* Features() const { return &m_physicalDeviceFeatures; }
    const VkPhysicalDeviceProperties2* Properties() { return &m_physicalDeviceProperties; }
    const VkPhysicalDeviceMemoryProperties2* MemoryProperties() { return &m_physicalDeviceMemoryProperties; }
    const VkPhysicalDeviceFeatures2* Features() const { return &m_physicalDeviceFeatures; }


protected:



private:

    ClVkPhysicalDevice() {};

    static ClVkPhysicalDevice m_ClVkPhysicalDevice;
    VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };

    // VkPhysicalDeviceProperties m_physicalDeviceProperties;
    // VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;
    // VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
    VkPhysicalDeviceProperties2KHR m_physicalDeviceProperties;
    VkPhysicalDeviceMemoryProperties2KHR m_physicalDeviceMemoryProperties;
    VkPhysicalDeviceFeatures2KHR m_physicalDeviceFeatures;

    u32 m_deviceCount;
    // std::multimap<i32, VkPhysicalDevice> m_candidates;
    std::vector<VkPhysicalDevice> m_physicalDeviceList;

    b8 IsDeviceSuitable(const VkPhysicalDevice& PhysDevice);
    u32 RateDeviceSuitability(const VkPhysicalDevice& PhysDevice);
    b8 CheckDeviceExtensionSupport(const VkPhysicalDevice& PhysDevice);

    void SetPropertiesAndFeatures(const VkPhysicalDevice& PhysDevice);

public:

    /**
     * @brief (vkGetPhysicalDeviceFormatProperties wrapper) Get the VkPhysicalDeviceFormatProperties object
     * @param Format VkFormat to query
     * @param pFormatProps VkPhysicalDeviceFormatProperties* handle to set
     */
    void GetPhysDeviceFormatProperties(const VkFormat& Format, VkFormatProperties* pFormatProps) { vkGetPhysicalDeviceFormatProperties(m_physicalDevice, Format, pFormatProps); }

};