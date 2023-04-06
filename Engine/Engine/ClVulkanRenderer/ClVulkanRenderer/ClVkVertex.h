#pragma once

#include <PCH/pch.h>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <functional>

#include <ClMath/Vec2.h>
#include <ClMath/Vec3.h>


struct Vertex3d
{
    // glm::vec3 pos;
    // glm::vec3 color;
    // glm::vec2 texCoord;
    Clunk::Vec3 pos;
    Clunk::Vec3 color;
    Clunk::Vec2 texCoord;

    static VkVertexInputBindingDescription GetBindDesc()
    {
        VkVertexInputBindingDescription bindingDesc{};
        bindingDesc.binding = 0;
        bindingDesc.stride = sizeof(Vertex3d);
        bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDesc;
    }

    static std::array<VkVertexInputAttributeDescription, 3> GetAttibDesc()
    {
        std::array<VkVertexInputAttributeDescription, 3> attribDesc{};
        // pos
        attribDesc[0].binding = 0;
        attribDesc[0].location = 0;
        attribDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT,
        attribDesc[0].offset = offsetof(Vertex3d, pos);
        // color
        attribDesc[1].binding = 0;
        attribDesc[1].location = 1;
        attribDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribDesc[1].offset = offsetof(Vertex3d, color);
        // texCoord
        attribDesc[2].binding = 0;
        attribDesc[2].location = 2;
        attribDesc[2].format = VK_FORMAT_R32G32_SFLOAT,
        attribDesc[2].offset = offsetof(Vertex3d, texCoord);

        return attribDesc;
    }

    bool operator==(const Vertex3d& other) const
    {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }

    bool operator==(const Vertex3d& other)
    {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

namespace std
{
    template<> struct hash<Vertex3d>
    {
        size_t operator()(const Vertex3d& V) const
        {
            return ((hash<Clunk::Vec3>()(V.pos) ^
                    (hash<Clunk::Vec3>()(V.color) << 1)) >> 1) ^
                    (hash<Clunk::Vec2>()(V.texCoord) << 1);
        }
    };
}


const std::vector<Vertex3d> VERTICES = 
{
    // {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    // {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    // {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    // {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    // {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    // {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    // {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    // {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}

    { Clunk::Vec3(-0.5f, -0.5f, 0.0f), Clunk::Vec3(1.0f, 0.0f, 0.0f), Clunk::Vec2(0.0f, 0.0f)},
    { Clunk::Vec3(0.5f, -0.5f, 0.0f), Clunk::Vec3(0.0f, 1.0f, 0.0f), Clunk::Vec2(1.0f, 0.0f)},
    { Clunk::Vec3(0.5f, 0.5f, 0.0f), Clunk::Vec3(0.0f, 0.0f, 1.0f), Clunk::Vec2(1.0f, 1.0f)},
    { Clunk::Vec3(-0.5f, 0.5f, 0.0f), Clunk::Vec3(1.0f, 1.0f, 1.0f), Clunk::Vec2(0.0f, 1.0f)},

    {Clunk::Vec3(-0.5f, -0.5f, -0.5f), Clunk::Vec3(1.0f, 0.0f, 0.0f), Clunk::Vec2(0.0f, 0.0f)},
    {Clunk::Vec3(0.5f, -0.5f, -0.5f), Clunk::Vec3(0.0f, 1.0f, 0.0f), Clunk::Vec2(1.0f, 0.0f)},
    {Clunk::Vec3(0.5f, 0.5f, -0.5f), Clunk::Vec3(0.0f, 0.0f, 1.0f), Clunk::Vec2(1.0f, 1.0f)},
    {Clunk::Vec3(-0.5f, 0.5f, -0.5f), Clunk::Vec3(1.0f, 1.0f, 1.0f), Clunk::Vec2(0.0f, 1.0f)}
};

const std::vector<u16> INDICES = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
};