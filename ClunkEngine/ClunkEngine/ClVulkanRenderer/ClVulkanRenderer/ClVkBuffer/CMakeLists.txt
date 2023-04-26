set(Target ClVkBuffer)

file(GLOB SRC_FILES 
    src/*.cpp
)

file(GLOB HDR_FILES
    include/${Target}/*h
)

add_library(${Target}
    ${SRC_FILES}
)

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
    VkMemAllocator
    PCH
    Defines
    Core
    ClVkContext
)

target_compile_features(${Target} PUBLIC cxx_std_17)