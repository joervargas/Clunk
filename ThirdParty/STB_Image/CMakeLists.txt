add_library(STB_Image
    src/stb_image.cpp
    src/stb_image_write.cpp
)

target_include_directories(STB_Image
    PUBLIC 
        include
        src
)

target_compile_features(STB_Image PUBLIC cxx_std_17)