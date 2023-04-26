#include "ClVkPipelineLayout.h"

#include <ClVkContext/ClVkDevice.h>
#include <Core/Exception.h>
#include <Core/Logger.h>

void ClVkPipelineLayout::Create(const VkDescriptorSetLayout* pDescSetLayout, const VkPushConstantRange* pPushConstantRange)
{
    VkPipelineLayoutCreateInfo layoutCI{};
    layoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    if(pDescSetLayout)
    {
        layoutCI.setLayoutCount = 1;
        layoutCI.pSetLayouts = pDescSetLayout;
    }
    else
    {
        layoutCI.setLayoutCount = 0;
        layoutCI.pSetLayouts = nullptr;
    }
    if(pPushConstantRange)
    {
        layoutCI.pushConstantRangeCount = 1;
        layoutCI.pPushConstantRanges = pPushConstantRange;
    }
    else
    {
        layoutCI.pushConstantRangeCount = 0;
        layoutCI.pPushConstantRanges = nullptr;
    }

    ClVkDevice* Device = ClVkDevice::Get();
    VK_CHECK(Device->CreatePiplineLayout(&layoutCI, nullptr, &m_layout));
}

void ClVkPipelineLayout::Destroy()
{
    CLOG_INFO("Destroying Vk PipelineLayout...");

    ClVkDevice* Device = ClVkDevice::Get();
    Device->DestroyPipelineLayout(m_layout, nullptr); 

    CLOG_INFO("Vk PipelineLayout Destroyed.");
}
