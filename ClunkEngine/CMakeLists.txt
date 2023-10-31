project(ClunkEngine VERSION 0.0.1)


file(GLOB SRC_FILES 
    ${PROJECT_NAME}/*.cpp
)

file(GLOB HDR_FILES
    ${PROJECT_NAME}/*.h
)


set(Engine_DIR ./ClunkEngine)
add_subdirectory(${Engine_DIR}/PCH)
add_subdirectory(${Engine_DIR}/ClMath)
add_subdirectory(${Engine_DIR}/Events)
add_subdirectory(${Engine_DIR}/ClPlatforms)
add_subdirectory(${Engine_DIR}/ClRenderManager)
add_subdirectory(${Engine_DIR}/ClScene)
add_subdirectory(${Engine_DIR}/ClAssets)
add_subdirectory(${Engine_DIR}/Core)


if(${VULKAN_GRAPHICS})
    add_compile_definitions(VK_GFX)
    add_subdirectory(${Engine_DIR}/ClVulkanRenderer)
    message("Vulkan found in ${Target}!")
else()
    message("***Vulkan not found in ${Target}***")
endif()


add_library(${PROJECT_NAME}
    ${SRC_FILES}
)

target_sources(${PROJECT_NAME}
    PUBLIC
        ${HDR_FILES}
    PRIVATE
        ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_NAME}/
)

target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2
    SDL2::SDL2main
    TinyXML2
    TinyObjLoader
    PCH
    ClMath
    ClPlatforms
    ClRenderManager
    ClScene
    ClAssets
    Core
    Events
)

if(${VULKAN_GRAPHICS})
    target_link_libraries(${PROJECT_NAME}
        # vulkan
        ClVulkan
        ClVulkanRenderer
    )
endif()
