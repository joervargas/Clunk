set(Target Events)

file(GLOB SRC_FILES
    ${Target}/*.cpp
)

file(GLOB HDR_FILES
    ${Target}/*.h
)

# if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
#     find_library(SDL2::SDL2 ${ENGINE_LIB_DIR})
# endif()

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
    SDL2::SDL2
    PCH
)

target_compile_features(${Target} PUBLIC cxx_std_17)