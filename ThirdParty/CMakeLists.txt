set(Target ThirdParty)

include(FetchContent)

# set(CMAKE_DEBUG_POSTFIX "")
message("Fetching Engine Content...")
message("Fetching SDL2...")
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.26.3
)
FetchContent_MakeAvailable(SDL2)

# add_subdirectory(./SDL2)
# if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    # list(APPEND ${CMAKE_PREFIX_PATH} ${SDL2_BINARY_DIR})
# endif()

find_package(glm REQUIRED)
if(glm_FOUND)
    message("GLM found!")
else()
    message("***GLM not found***")
endif()
    
find_package(Vulkan REQUIRED)
if(VULKAN_FOUND)
    # add_compile_definitions(VK_NO_PROTOTYPES)
    set(VULKAN_GRAPHICS ON PARENT_SCOPE)
    message("Vulkan found!")
else()
    Set(VULKAN_GRAPHICS OFF PARENT_SCOPE)
    message("***Vulkan not found***")
endif()

# add_subdirectory(./ImGui)
add_subdirectory(./Json)
add_subdirectory(./STB_Image)
add_subdirectory(./TinyGLTF)
add_subdirectory(./TinyObjLoader)
add_subdirectory(./TinyXML2)
add_subdirectory(./MeshOptimizer)


# add_custom_target(${Target}
#     DEPENDS
#         # SDL2
#         SD2::SDL2
#         Vulkan::Vulkan
#         ImGui
#         Json
#         STB_Image
#         TinyGLTF
#         TinyObjLoader
#         TinyXML2
#         VkMemAllocator
#         MeshOptimizer
# )


# if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
#     list(APPEND ${CMAKE_PREFIX_PATH} ${ENGINE_LIB_DIR})
#     add_custom_target(
#         # TARGET ${Target} POST_BUILD
#         ${Target}
#         COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "${ENGINE_LIB_DIR}"
#         DEPENDS SDL2::SDL2
#         VERBATIM
#     )
#     find_library(SDL2::SDL2 ${ENGINE_LIB_DIR})
# endif()