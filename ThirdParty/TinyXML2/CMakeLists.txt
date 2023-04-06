add_library(TinyXML2
    src/tinyxml2.cpp
)

target_include_directories(TinyXML2
    PUBLIC 
        include
)

target_compile_features(TinyXML2 PUBLIC cxx_std_17)