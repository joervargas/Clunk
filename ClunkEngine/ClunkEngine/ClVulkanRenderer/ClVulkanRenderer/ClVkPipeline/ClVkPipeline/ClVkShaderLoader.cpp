#include "ClVkShaderLoader.h"
#include <ClVkContext/ClVkDevice.h>

#include <Core/Logger.h>
#include <Core/Exception.h>


ClVkShaderLoader::ClVkShaderLoader()
{
}

ClVkShaderLoader::~ClVkShaderLoader()
{
}

void ClVkShaderLoader::SetStage(const String &FileName, VkShaderStageFlagBits ShaderStageFlag)
{
    auto code = Read(FileName);
    VkShaderModule shaderModule = CreateModule(code);

    switch (ShaderStageFlag)
    {
    case VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT :
        m_vertModule = shaderModule;
        break;
    case VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT :
        m_fragModule = shaderModule;
        break;
    default:
        THROW_EXCEPTION("Shader Stage not supported.");
        break;
    }
}

void ClVkShaderLoader::Destroy()
{
    ClVkDevice* Device = ClVkDevice::Get();
    if(m_vertModule.has_value())
    {
        // vkDestroyShaderModule(*Device->Get(), m_vertModule.value(), nullptr);
        Device->DestroyShaderModule(m_vertModule.value(), nullptr);
        m_vertModule.reset();
    }
    if(m_fragModule.has_value())
    {
        // vkDestroyShaderModule(*Device->Get(), m_fragModule.value(), nullptr);
        Device->DestroyShaderModule(m_fragModule.value(), nullptr);
        m_fragModule.reset();
    }
}

void ClVkShaderLoader::PopulateShaderStages()
{
    m_shaderStagesCI.clear();
    VkPipelineShaderStageCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    ci.pName = "main";

    if(m_vertModule.has_value())
    {
        ci.stage = VK_SHADER_STAGE_VERTEX_BIT;
        ci.module = m_vertModule.value();
        m_shaderStagesCI.push_back(ci);
    }
    if(m_fragModule.has_value())
    {
        ci.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        ci.module = m_fragModule.value();
        m_shaderStagesCI.push_back(ci);
    }
}

VkShaderModule ClVkShaderLoader::CreateModule(const std::vector<char> &code)
{
    VkShaderModuleCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    ci.codeSize = code.size();
    ci.pCode = reinterpret_cast<const u32*>(code.data());

    VkShaderModule shaderModule;
    ClVkDevice* Device = ClVkDevice::Get();
    // VK_CHECK(vkCreateShaderModule(*Device->Get(), &ci, nullptr, &shaderModule));
    VK_CHECK(Device->CreateShaderModule(&ci, nullptr, &shaderModule));

    return shaderModule;
}

std::vector<char> ClVkShaderLoader::Read(const String &FileName)
{
    // ate: Read from 'at the end' of the file
    // binary: Read the file as binary
    std::ifstream file(FileName, std::ios::ate | std::ios::binary);

    if(!file.is_open())
    {
        THROW_EXCEPTION("Failed to open Shader file: %s", FileName.c_str());
    }

    // Since the cursor is at the end of the file
    // We can quickly get its file size
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    // Go back to the begining of the file
    file.seekg(0);
    // Read the entire file of bytes to the buffer
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}
