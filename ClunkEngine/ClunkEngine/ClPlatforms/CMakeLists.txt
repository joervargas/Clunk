set(Target ClPlatforms)

file(GLOB SRC_FILES 
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*.h
)

include_directories(../)
include_directories(../Core)
include_directories(../Events)
include_directories(../PCH)
include_directories(../Defines)

# if(${VULKAN_GRAPHICS})
#     add_compile_definitions(VK_GFX)
#     message("Vulkan found in ${Target}!")
# else()
#     message("***Vulkan not found in ${Target}***")
# endif()

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

add_library(${Target}
    ${SRC_FILES}
)

# target_sources(${Target}
#     PUBLIC
#         ${HDR_FILES}
#     PRIVATE
#         ${SRC_FILES}
# )

target_include_directories(${Target}
    PUBLIC
        ./
)

target_link_libraries(${Target}
    SDL2::SDL2
    SDL2::SDL2main
    PCH
    Events
    Core
)

if(${VULKAN_GRAPHICS})
    target_link_libraries(${Target}
        # ${Vulkan_LIBRARIES}
        Vulkan::Vulkan
        # ClVulkanRenderer
    )
endif()


# if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
#     add_custom_command(
#         TARGET ${Target} POST_BUILD
#         COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:${Target}>"
#         VERBATIM
#     )
# endif()

# if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
#     # list(APPEND ${CMAKE_PREFIX_PATH} ${ENGINE_LIB_DIR})
#     add_custom_command(
#         TARGET ${Target} POST_BUILD
#         COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "${PROJECT_BINARY_DIR}"
#         VERBATIM
#     )
# endif()