cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
# project(Clunk VERSION 0.1.0)

# find_package(SDL2 REQUIRED)
# if(SDL2_FOUND)
#     message("SDL2 found!")
# else()
#     message("***SDL2 not found***")
# endif()

# find_package(glm REQUIRED)
# if(glm_FOUND)
#     message("GLM found!")
# else()
#     message("***GLM not found***")
# endif()
    
# find_package(Vulkan REQUIRED)
# if(VULKAN_FOUND)
#     message("Vulkan found!")
# else()
#     message("***Vulkan not found***")
# endif()

# find_package(Ktx REQUIRED)
# if(Ktx_FOUND)
#     message("Ktx found!")
# else()
#     message("***Ktx not found***")
# endif()

# add_compile_definitions(VK_NO_PROTOTYPES)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ClunkEngine/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ClunkyEditor/)

# file(COPY Assets/ DESTINATION ${CMAKE_BINARY_DIR}/Assets)
# file(COPY Shaders/ DESTINATION ${CMAKE_BINARY_DIR}/Shaders)