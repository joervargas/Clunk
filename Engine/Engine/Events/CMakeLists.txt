set(Target Events)

file(GLOB SRC_FILES
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*.h
)

include_directories(../)
include_directories(../PCH)

add_library(${Target} 
    ${SRC_FILES}
)

# target_sources(${Target}
#     PUBLIC
#         ${HDR_FILES}
#     PRIVATE
#         ${SRC_FILES}
# )

target_include_directories(${Target}
    PUBLIC
        ./
)

target_link_libraries(${Target}
    vulkan
    Defines
    PCH
)

target_compile_features(${Target} PUBLIC cxx_std_17)