set(Target ClRenderManager)

file(GLOB SRC_FILES 
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*.h
)

include_directories(../)
# include_directories(../Defines)

if(${VULKAN_GRAPHICS})
    add_compile_definitions(VK_GFX)
    message("Vulkan found in ${Target}!")
else()
    message("***Vulkan not found in ${Target}***")
endif()

# if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
#     find_library(SDL2::SDL2 ${ENGINE_LIB_DIR})
# endif()

add_library(${Target}
    ${SRC_FILES}
)

# if(${VULKAN_GRAPHICS})
#     target_compile_definitions(${Target} PUBLIC VK_GFX)
#     message("Vulkan found in ClRenderManager!")
# else()
#     message("***Vulkan not found in Engine***")
# endif()

target_include_directories(${Target}
    PUBLIC
        ./
)

target_link_libraries(${Target}
    SDL2::SDL2
    TinyXML2
    PCH
    Core
)

if(${VULKAN_GRAPHICS})
    target_link_libraries(${Target}
       # vulkan
        ClVulkanRenderer
    )
endif()
