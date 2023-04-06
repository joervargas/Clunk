
add_library(TinyGLTF
    src/tiny_gltf.cpp
)


# include_directories(../libs/Json/include)
# include_directories(../libs/STB_Image/include)
include_directories(../Json/include)
include_directories(../STB_Image/include)

target_include_directories(TinyGLTF
    PUBLIC
        include
)

target_link_libraries(TinyGLTF
    Json
    STB_Image
)

target_compile_features(TinyGLTF PUBLIC cxx_std_11)