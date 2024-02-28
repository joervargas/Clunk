set(Target ClCamera)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(${Target}
    # glm::glm
    PCH
    ClPlatforms
    Core
)