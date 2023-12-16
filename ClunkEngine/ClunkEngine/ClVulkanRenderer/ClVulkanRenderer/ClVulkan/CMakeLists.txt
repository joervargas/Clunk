set(Target ClVulkan)

file(GLOB SRC_FILES 
    src/*.cpp
)

file(GLOB HDR_FILES
    include/${Target}/*h
)

add_library(${Target}
    ${SRC_FILES}
)

add_subdirectory(ClVkExt)

target_include_directories(${Target}
    PUBLIC
        include
    PRIVATE
        include/${Target}
        src
)

target_link_libraries(${Target}
    ClVkExt
    Vulkan::Vulkan
    SPIRV
    glslang
    glslang-default-resource-limits
    VkMemAllocator
    PCH
    Core
)
