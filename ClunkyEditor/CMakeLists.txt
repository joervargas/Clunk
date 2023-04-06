set(Target ClunkyEditor)

file(GLOB SRC_FILES 
    ClunkyEditor/*.cpp
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


set(Engine_DIR ../Engine)
include_directories(${Engine_DIR})


if(${VULKAN_GRAPHICS})
    # add_compile_definitions(VK_NO_PROTOTYPES)
    message("Vulkan found in ${Target}!")
else()
    message("***Vulkan not found in ${Target}***")
endif()

add_executable(${Target}
    ${SRC_FILES}
)

target_link_libraries(${Target}
    Engine
    ImGui
)

target_compile_features(${Target} PUBLIC cxx_std_17)