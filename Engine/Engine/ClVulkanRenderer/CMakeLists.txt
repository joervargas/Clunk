set(Target ClVulkanRenderer)

file(GLOB SRC_FILES 
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*.h
)

# add_subdirectory(${Target}/ClVkLoader)
add_subdirectory(${Target}/ClVkContext)
add_subdirectory(${Target}/ClVkImage)
add_subdirectory(${Target}/ClVkBuffer)
add_subdirectory(${Target}/ClVkDescriptor)
add_subdirectory(${Target}/ClVkSwapchain)
add_subdirectory(${Target}/ClVkPipeline)
add_subdirectory(${Target}/ClVkCmds)


add_library(${Target}
    ${SRC_FILES}
)


target_include_directories(${Target}
    PUBLIC
        ./
)

target_link_libraries(${Target}
    # ClVkLoader
    ${Vulkan_LIBRARIES}
    # VulkanMemoryAllocator
    SDL2
    ClPlatforms
    TinyXML2
    TinyObjLoader
    MeshOptimizer
    VkMemAllocator
    STB_Image
    Defines
    PCH
    Core
    ClMath
    ClVkExt
    ClVkContext
    ClVkImage
    ClVkBuffer
    ClVkDescriptor
    ClVkSwapchain
    ClVkPipeline
    ClVkCmds
    ClRenderManager
)

target_compile_features(${Target} PUBLIC cxx_std_17)