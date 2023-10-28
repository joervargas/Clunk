# set(Target ClunkyEditor)
project(ClunkyEditor)

file(GLOB SRC_FILES 
    ${PROJECT_NAME}/*.cpp
)


set(Engine_DIR ../ClunkEngine)
include_directories(${Engine_DIR})


if(${VULKAN_GRAPHICS})
    # add_compile_definitions(VK_NO_PROTOTYPES)
    message("Vulkan found in ClunkyEditor!")
    message("vulkan libraries: ${Vulkan_LIBRARY}")
else()
    message("***Vulkan not found in ClunkyEditor***")
endif()


add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME}
    ClunkEngine
    # ImGui
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Shaders" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    VERBATIM
)

# target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)