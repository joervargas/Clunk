#pragma once

#include <vulkan/vulkan.h>
#include <glslang/Include/glslang_c_interface.h>

#include <PCH/pch.h>


int ends_with(const char* Str, const char* Part);

String read_shader_file(const char* Filename);

void print_shader_source(const char* Text);

namespace Clunk::Vk
{
    struct ClVkShaderModule
    {
        std::vector<u32> SPIRV;
        VkShaderModule Handle = nullptr;
        VkShaderStageFlagBits Stage = VK_SHADER_STAGE_VERTEX_BIT;
    };

    glslang_stage_t get_glsl_stage_from_filename(const char* Filename);

    VkShaderStageFlagBits get_vk_shader_stage_from_glsl_stage(glslang_stage_t Stage);

    VkShaderStageFlagBits get_vk_shader_stage_from_filename(const char* Filename);

    size_t compile_glsl_shader(glslang_stage_t Stage, const char* Source, ClVkShaderModule* Module);

    size_t compile_glsl_shader_file(const char* Filename, ClVkShaderModule* Module);

    ClVkShaderModule cl_create_vk_shader_module(const VkDevice Device, const char* Filename);

    /*
     * @brief Returns a populated VkPipelineShaderStageCreateInfo struct
     * 
     * @param ShaderModule ClVkShaderModule
     * @param Stage VkShaderStageFlagBit
     * @param EntryPoint Shader entry point function name. (default is "main")
     * @return VkPipelineShaderStageCreateInfo 
     */
    inline VkPipelineShaderStageCreateInfo cl_get_vk_pipeline_shader_stage_create_info(ClVkShaderModule& ShaderModule, const char* EntryPoint = "main")
    {
        return VkPipelineShaderStageCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = ShaderModule.Stage,
            .module = ShaderModule.Handle,
            .pName = EntryPoint,
            .pSpecializationInfo = nullptr
        };
    }
}
