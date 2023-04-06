#pragma once


#include <vulkan/vulkan.h>
#include <Defines/Defines.h>


class ClVkPipelineLayout
{
public:

    ClVkPipelineLayout(){};
    ~ClVkPipelineLayout(){};

    void Create(const VkDescriptorSetLayout* pDescSetLayout, const VkPushConstantRange* pPushConstantRange);
    void Destroy();

    VkPipelineLayout* Handle() { return &m_layout; }

private:

    VkPipelineLayout m_layout;

};