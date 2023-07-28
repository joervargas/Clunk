set(Target VkMemAllocator)

add_library(${Target}
    src/vk_mem_alloc.cpp
)

target_include_directories(${Target}
    PUBLIC 
        include
        src
)

target_link_libraries(${Target} Vulkan::Vulkan)
