set(Target Engine)

file(GLOB SRC_FILES 
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*.h
)

set(Engine_DIR ./Engine)
add_subdirectory(${Engine_DIR}/Defines)
add_subdirectory(${Engine_DIR}/PCH)
add_subdirectory(${Engine_DIR}/ClMath)
add_subdirectory(${Engine_DIR}/Events)
add_subdirectory(${Engine_DIR}/ClPlatforms)
add_subdirectory(${Engine_DIR}/ClRenderManager)
add_subdirectory(${Engine_DIR}/Core)

# find_package(Vulkan REQUIRED)
if(${VULKAN_GRAPHICS})
    add_compile_definitions(VK_GFX)
    # add_compile_definitions(VK_NO_PROTOTYPES)
    add_subdirectory(${Engine_DIR}/ClVulkanRenderer)
    message("Vulkan found in ${Target}!")
else()
    message("***Vulkan not found in ${Target}***")
endif()

add_library(${Target}
    ${SRC_FILES}
)

target_sources(${Target}
    PUBLIC
        ${HDR_FILES}
    PRIVATE
        ${SRC_FILES}
)
target_include_directories(${Target}
    PUBLIC
        ${Target}/
)

target_link_libraries(${Target}
    SDL2
    SDL2_image
    TinyXML2
    TinyObjLoader
    Defines
    ClMath
    ClPlatforms
    ClRenderManager
    Core
    Events
)

if(${VULKAN_GRAPHICS})
    target_link_libraries(${Target}
        # vulkan
        ClVulkanRenderer
    )
endif()