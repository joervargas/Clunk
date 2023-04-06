set(Target ImGui)

file(GLOB SRC_FILES 
    src/*.cpp
)

# file(GLOB HDR_FILES
#     ${Target}/*.h
# )

add_library(${Target}
    ${SRC_FILES}
)

target_include_directories(${Target}
    PUBLIC 
        include
    PRIVATE
        include/${Target}
)

target_compile_features(${Target} PUBLIC cxx_std_17)