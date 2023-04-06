add_library(TinyObjLoader
    src/tiny_obj_loader.cpp
)

target_include_directories(TinyObjLoader
    PUBLIC 
        include
)

target_compile_features(TinyObjLoader PUBLIC cxx_std_17)