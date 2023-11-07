set(Target ClVkRenderLayers)

file(GLOB SRC_FILES 
    src/*.cpp
)

file(GLOB HDR_FILES
    include/${Target}/*h
)

add_library(${Target}
    ${SRC_FILES}
)

# add_subdirectory(ClVkExt)
include_directories(../ClVulkan/include)
# include_directories(../../libs/VkMemAllocator/include)

link_directories(../ClVulkan)

target_include_directories(${Target}
    PUBLIC
        include
    PRIVATE
        include/${Target}
        src
)

target_link_libraries(${Target}
    # ClVkExt
    # Vulkan::Vulkan
    PCH
    ClVulkan
    # SPIRV
    # glslang
    # glslang-default-resource-limits
    # VkMemAllocator
    ClMath
    Core
)
