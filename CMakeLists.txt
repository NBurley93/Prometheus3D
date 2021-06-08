cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(Prometheus3D)

add_library(StarCore STATIC)
add_library(StarRender STATIC)
add_executable(Prometheus3D
    StarGL.rc
)

add_subdirectory(StarCore)
add_subdirectory(StarRender)
add_subdirectory(src)

# Vendor libs
add_subdirectory(vendor)

target_include_directories(StarCore PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(StarCore PUBLIC
    SDL2::SDL2
)

target_include_directories(StarRender PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/StarCore/src
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/vendor/freetype2/include
    ${CMAKE_SOURCE_DIR}/vendor/glad/include
    ${CMAKE_SOURCE_DIR}/vendor/soil/inc
    ${CMAKE_SOURCE_DIR}/vendor/glm
)

find_package(OpenGL REQUIRED)

target_link_libraries(StarRender PUBLIC
    glad
    freetype
    assimp
    soil
    StarCore
    glm::glm
    OpenGL::GL
)

target_include_directories(Prometheus3D PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/StarCore/src
    ${CMAKE_SOURCE_DIR}/StarRender/src
)

target_link_libraries(Prometheus3D PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/SDL2_mixer.lib
    StarRender
    SDL2::SDL2main
)