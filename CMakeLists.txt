cmake_minimum_required(VERSION 3.19)

project(Clunk)

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

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ClunkyEditor/)

file(COPY Assets/ DESTINATION ${CMAKE_BINARY_DIR}/Assets)
file(COPY Shaders/ DESTINATION ${CMAKE_BINARY_DIR}/Shaders)