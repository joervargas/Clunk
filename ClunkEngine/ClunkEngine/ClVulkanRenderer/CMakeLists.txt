set(Target ClVulkanRenderer)

file(GLOB SRC_FILES 
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*.h
)

find_package(Vulkan REQUIRED)
if(VULKAN_FOUND)
    # add_compile_definitions(VK_NO_PROTOTYPES)
    set(VULKAN_GRAPHICS ON PARENT_SCOPE)
    message("Vulkan found!: $ENV{VULKAN_SDK}")
else()
    Set(VULKAN_GRAPHICS OFF PARENT_SCOPE)
    message("***Vulkan not found***")
endif()

# if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
#     find_library(SDL2::SDL2 ${ENGINE_LIB_DIR})
# endif()

# add_subdirectory(${Target}/ClVkLoader)
add_subdirectory(${Target}/ClVulkan)
# add_subdirectory(${Target}/ClVkImage)
# add_subdirectory(${Target}/ClVkBuffer)
# add_subdirectory(${Target}/ClVkDescriptor)
# add_subdirectory(${Target}/ClVkSwapchain)
# add_subdirectory(${Target}/ClVkPipeline)
# add_subdirectory(${Target}/ClVkCmds)


add_library(${Target}
    ${SRC_FILES}
)


target_include_directories(${Target}
    PUBLIC
        ./
)

target_link_libraries(${Target}
    SDL2::SDL2
    Vulkan::Vulkan
    # SPIRV-Tools
    glslang
    glslang-default-resource-limits
    ClPlatforms
    TinyXML2
    TinyObjLoader
    MeshOptimizer
    VkMemAllocator
    STB_Image
    PCH
    Core
    ClMath
    ClVkExt
    ClVulkan
    ClRenderManager
)
