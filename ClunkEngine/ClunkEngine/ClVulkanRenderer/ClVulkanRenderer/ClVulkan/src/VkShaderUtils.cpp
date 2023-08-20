#include "VkShaderUtils.h"
#include "VkDefines.h"

#include <glslang/Public/resource_limits_c.h>
// #include <glslang/Public/ShaderLang.h>

#include <string.h>


int ends_with(const char *Str, const char *Part)
{
    return (strstr(Str, Part) - Str) == (strlen(Str) - strlen(Part));
}

String read_shader_file(const char *Filename)
{
    FILE* file = fopen(Filename, "r");
    if(!file)
    {
        THROW_EXCEPTION("I/O error. Cannot open '%s'\n", Filename);
        return String();
    }

    fseek(file, 0L, SEEK_END);
    const auto bytes_in_file = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char* buffer = (char*)alloca(bytes_in_file + 1);
    const size_t bytes_read = fread(buffer, 1, bytes_in_file, file);
    fclose(file);

    buffer[bytes_read] = 0;
    static constexpr u8 BOM[] = { 0xEF, 0xBB, 0xBF};
    if(bytes_read > 3)
    {
        if(!memcmp(buffer, BOM, 3))
        {
            memset(buffer, ' ', 3);
        }
    }

    String code(buffer);

    while(code.find("#include ") != code.npos)
    {
        const auto pos = code.find("#include ");
        const auto p1 = code.find('<', pos);
        const auto p2 = code.find('>', pos);
        if(p1 == code.npos || p2 == code.npos || p2 <= p1)
        {
            THROW_EXCEPTION("Error while loading the shader program: %s\n", code.c_str());
            return String();
        }

        const String name = code.substr(p1 + 1, p2 - p1 -1);
        const String include = read_shader_file(name.c_str());
        code.replace(pos, p2-pos + 1, include.c_str());
    }

    return code;
}

void print_shader_source(const char *Text)
{
    i32 line = 1;
    printf("\n(%3i) ", line);
    while(Text && *Text++)
    {
        if(*Text == '\n')
        {
            printf("\n(%3i) ", ++line);
        }
        else if (*Text == '\r')
        {}
        else
        {
            printf("%c", *Text);
        }
    }
    printf("\n");
}

namespace Clunk::Vk
{
    glslang_stage_t get_glsl_stage_from_filename(const char *Filename)
    {
        if(ends_with(Filename, ".vert")) { return GLSLANG_STAGE_VERTEX; }
        if(ends_with(Filename, ".frag")) { return GLSLANG_STAGE_FRAGMENT; }
        if(ends_with(Filename, ".comp")) { return GLSLANG_STAGE_COMPUTE; }
        if(ends_with(Filename, ".geom")) { return GLSLANG_STAGE_GEOMETRY; }
        if(ends_with(Filename, ".tesc")) { return GLSLANG_STAGE_TESSCONTROL; }
        if(ends_with(Filename, ".tese")) { return GLSLANG_STAGE_TESSEVALUATION; }

        return GLSLANG_STAGE_VERTEX;
    }

    VkShaderStageFlagBits get_vk_shader_stage_from_glsl_stage(glslang_stage_t Stage)
    {
        switch (Stage)
        {
        case GLSLANG_STAGE_VERTEX:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case GLSLANG_STAGE_FRAGMENT:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case GLSLANG_STAGE_COMPUTE:
            return VK_SHADER_STAGE_COMPUTE_BIT;
        case GLSLANG_STAGE_GEOMETRY:
            return VK_SHADER_STAGE_GEOMETRY_BIT;
        case GLSLANG_STAGE_TESSCONTROL:
            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case GLSLANG_STAGE_TESSEVALUATION:
            return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        default:
            return VK_SHADER_STAGE_VERTEX_BIT;
        }
    }

    VkShaderStageFlagBits get_vk_shader_stage_from_filename(const char *Filename)
    {
        return get_vk_shader_stage_from_glsl_stage(get_glsl_stage_from_filename(Filename));
    }

    size_t compile_glsl_shader(glslang_stage_t Stage, const char *Source, ClVkShaderModule *Module)
    {
        const glslang_input_t input =
        {
            .language = GLSLANG_SOURCE_GLSL,
            .stage = Stage,
            .client = GLSLANG_CLIENT_VULKAN,
            .client_version = GLSLANG_TARGET_VULKAN_1_3,
            .target_language = GLSLANG_TARGET_SPV,
            .target_language_version = GLSLANG_TARGET_SPV_1_5,
            .code = Source,
            .default_version = 100,
            .default_profile = GLSLANG_NO_PROFILE,
            .force_default_version_and_profile = false,
            .forward_compatible = false,
            .messages = GLSLANG_MSG_DEFAULT_BIT,
            .resource = glslang_default_resource()
        };

        glslang_shader_t* shader = glslang_shader_create(&input);
        if(!glslang_shader_preprocess(shader, &input))
        {
            CLOG_ERROR("GLSL preprocessing failed\n%s\n%s", glslang_shader_get_info_log(shader), glslang_shader_get_info_debug_log(shader));
            print_shader_source(input.code);
            return 0;
        }

        if(!glslang_shader_parse(shader, &input))
        {
            CLOG_ERROR("GLSL parsing failed\n%s\n%s", glslang_shader_get_info_log(shader), glslang_shader_get_info_debug_log(shader));
            print_shader_source(glslang_shader_get_preprocessed_code(shader));
            return 0;
        }

        glslang_program_t* program = glslang_program_create();
        glslang_program_add_shader(program, shader);

        if(!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT))
        {
            CLOG_ERROR("GLSL linking failed\n%s\n%s", glslang_program_get_info_log(program), glslang_program_get_info_debug_log(program));
            return 0;
        }

        glslang_program_SPIRV_generate(program, Stage);

        Module->SPIRV.resize(glslang_program_SPIRV_get_size(program));
        glslang_program_SPIRV_get(program, Module->SPIRV.data());

        const char* spirv_messages = glslang_program_SPIRV_get_messages(program);
        if(spirv_messages) { CLOG_ERROR("%s", spirv_messages); }

        glslang_program_delete(program);
        glslang_shader_delete(shader);

        return Module->SPIRV.size();
    }

    size_t compile_glsl_shader_file(const char *Filename, ClVkShaderModule *Module)
    {
        String source = read_shader_file(Filename);
        if(!source.empty())
        {
            glslang_stage_t stage = get_glsl_stage_from_filename(Filename);
            return compile_glsl_shader(stage, source.c_str(), Module);
        }
        return 0;
    }

    ClVkShaderModule cl_create_vk_shader_module(const VkDevice Device, const char *Filename)
    {
        ClVkShaderModule shader_module;
        if(!compile_glsl_shader_file(Filename, &shader_module)) { THROW_EXCEPTION("Failed to compile shader!"); }
        const VkShaderModuleCreateInfo create_info =
        {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = shader_module.SPIRV.size() * sizeof(u32),
            .pCode = shader_module.SPIRV.data()
        };
        VK_CHECK(vkCreateShaderModule(Device, &create_info, nullptr, &shader_module.Handle));

        return shader_module;
    }
}