set(Target ClVkExt)

file(GLOB SRC_FILES
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*h
    ${Target}/*inl
)

add_library(${Target}
    ${SRC_FILES}
)

target_include_directories(${Target}
    PUBLIC
        ./
    PRIVATE
        {Target}/
)

target_link_libraries(${Target}
    # Vulkan::Vulkan
    ${Vulkan_LIBRARY}
    PCH
    Core
)

# target_compile_features(${Target} PUBLIC cxx_std_17)