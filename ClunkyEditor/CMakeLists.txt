# set(Target ClunkyEditor)
project(ClunkyEditor)

file(GLOB SRC_FILES 
    ${PROJECT_NAME}/*.cpp
)

# include(FetchContent)
# message("Fetching Editor Content...")
# message("Fetching imgui...")
# FetchContent_Declare(
#     imgui
#     GIT_REPOSITORY https://github.com/ocornut/imgui.git
#     GIT_TAG 8d0723c2c36c7200d317fe1285ab86d24068c342
# )

# add_subdirectory(imgui)

# FetchContent_GetProperties(imgui)
# if(NOT imgui_POPULATED)
#     FetchContent_Populate(imgui)
# endif()

# add_subdirectory(imgui
#     ${imgui_SOURCE_DIR}/imgui.cpp
#     ${imgui_SOURCE_DIR}/imgui.h
#     ${imgui_SOURCE_DIR}/imconfig.h
#     ${imgui_SOURCE_DIR}/imgui_demo.cpp
#     ${imgui_SOURCE_DIR}/imgui_draw.cpp
#     ${imgui_SOURCE_DIR}/imgui_internal.h
#     ${imgui_SOURCE_DIR}/imgui_tables.cpp
#     ${imgui_SOURCE_DIR}/imgui_widgets.cpp
#     ${imgui_SOURCE_DIR}/imstb_rectpack.h
#     ${imgui_SOURCE_DIR}/imstb_textedit.h
#     ${imgui_SOURCE_DIR}/imstb_truetype.h
#     ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.h
#     ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
#     ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.h
#     ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
# )

# FetchContent_MakeAvailable(imgui)


set(Engine_DIR ../ClunkEngine)
include_directories(${Engine_DIR})


if(${VULKAN_GRAPHICS})
    # add_compile_definitions(VK_NO_PROTOTYPES)
    message("Vulkan found in ${Target}!")
    message("vulkan libraries: ${Vulkan_LIBRARY}")
else()
    message("***Vulkan not found in ${Target}***")
endif()


add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

# add_dependencies(${Target} Engine)

target_link_libraries(${PROJECT_NAME}
    ClunkEngine
    # ImGui
)

# if(${VULKAN_GRAPHICS})
#     target_link_libraries(${Target} ${Vulkan_LIBRARY})
# endif()

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        VERBATIM
    )
endif()

add_custom_command(
    TARGET ${PROJECT_NAME}
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Shaders" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    VERBATIM
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)