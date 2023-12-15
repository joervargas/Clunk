set(Target PCH)

file(GLOB SRC_FILES
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*.h
)

add_library(${Target} 
    ${SRC_FILES}
)

# target_sources(${Target}
#     PUBLIC
#         ${HDR_FILES}/PCH/
#     PRIVATE
#         ${SRC_FILES}
# )


target_include_directories(${Target}
    PUBLIC
        ./
)

target_link_libraries(${Target}
    # ClMath
)

if(${VULKAN_GRAPHICS})
    target_compile_definitions(${Target} PUBLIC VK_GFX)
    message("Vulkan found in ${Target}!")
else()
    message("***Vulkan not found in ${Target}***")
endif()

target_precompile_headers(${Target} PRIVATE ${HDR_FILES})