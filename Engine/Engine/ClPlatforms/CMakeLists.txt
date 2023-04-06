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

if(${VULKAN_GRAPHICS})
    add_compile_definitions(VK_GFX)
    message("Vulkan found in ${Target}!")
else()
    message("***Vulkan not found in ${Target}***")
endif()

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
    # vulkan
    SDL2
    Events
    Core
    Defines
)

# if(${VULKAN_GRAPHICS})
#     target_link_libraries(${Target}
#         vulkan
#         ClVulkanRenderer
#     )
# endif()