set(Target MeshOptimizer)

file(GLOB SRC_FILES 
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*.h
)

add_library(${Target}
    ${SRC_FILES}
)

target_include_directories(${Target}
    PUBLIC 
        ./
    PRIVATE
        ${Target}
)

target_compile_features(${Target} PUBLIC cxx_std_17)