set(Target ClVkSwapchain)

file(GLOB SRC_FILES 
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*h
)

add_library(${Target}
    ${SRC_FILES}
)

# include_directories(../Constants/include)
# include_directories(../Globals/include)
# include_directories(../../libs/VkMemAllocator/include)

target_include_directories(${Target}
    PUBLIC
        ./
    PRIVATE
        ${Target}
        # src
)

target_link_libraries(${Target}
    # VkMemAllocator
    ClPlatforms
    PCH
    Defines
    Core
    ClVkImage
)

target_compile_features(${Target} PUBLIC cxx_std_17)