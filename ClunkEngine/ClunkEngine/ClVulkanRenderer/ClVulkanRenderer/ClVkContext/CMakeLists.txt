set(Target ClVkContext)

file(GLOB SRC_FILES 
    src/*.cpp
)

file(GLOB HDR_FILES
    include/${Target}/*h
)

add_library(${Target}
    ${SRC_FILES}
)

add_subdirectory(ClVkExt)
# include_directories(../Constants/include)
# include_directories(../Globals/include)
# include_directories(../../libs/VkMemAllocator/include)

target_include_directories(${Target}
    PUBLIC
        include
    PRIVATE
        include/${Target}
        src
)

target_link_libraries(${Target}
    ClVkExt
    Vulkan::Vulkan
    VkMemAllocator
    PCH
    Core
)
