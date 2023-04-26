set(Target Defines)

# file(GLOB SRC_FILES 
#     Defines/*.cpp
# )

# file(GLOB HDR_FILES 
#     Defines/*.h
# )

if(${VULKAN_GRAPHICS})
    message("Vulkan found in ${Target}!")
    add_compile_definitions(VK_GFX)
else()
    message("***Vulkan not found in ${Target}***")
endif()

add_definitions(-DBINARY_DIR=${ClunkEngine_BINARY_DIR})

add_library(${Target} INTERFACE)
# add_library(${Target} ${SRC_FILES})

target_include_directories(${Target}
    INTERFACE
    # PUBLIC
        ./
)