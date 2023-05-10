set(Target ClAssets)

file(GLOB SRC_FILES 
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*.h
)

include_directories(../)

add_library(${Target}
    ${SRC_FILES}
)

target_include_directories(${Target}
    PUBLIC
        ./
)

target_link_libraries(${Target}
    PCH
)

target_compile_features(${Target} PUBLIC cxx_std_17)