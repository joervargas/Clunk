add_library(VkMemAllocator
    src/vk_mem_alloc.cpp
)

target_include_directories(VkMemAllocator 
    PUBLIC 
        include
        src
)

target_compile_features(VkMemAllocator PUBLIC cxx_std_17)