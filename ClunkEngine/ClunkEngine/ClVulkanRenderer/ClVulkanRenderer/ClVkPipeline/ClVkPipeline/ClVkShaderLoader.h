#pragma once

#include <vulkan/vulkan.h>

#include <optional>

#include <PCH/pch.h>


class ClVkShaderLoader
{
public:

    ClVkShaderLoader();
    ~ClVkShaderLoader();

    void SetStage(const String& FileName, VkShaderStageFlagBits ShaderStageFlag);

    void Destroy();

    void PopulateShaderStages();

    std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages() { return m_shaderStagesCI; }
    u32 GetShaderStagesCount() const { return (u32)m_shaderStagesCI.size(); }

private:

    std::optional<VkShaderModule> m_vertModule;
    std::optional<VkShaderModule> m_fragModule;
    
    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStagesCI;

    VkShaderModule CreateModule(const std::vector<char>& code);

    std::vector<char> Read(const String& FileName);
};