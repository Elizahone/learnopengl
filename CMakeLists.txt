cmake_minimum_required(VERSION 3.21..3.51)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(learnopengl LANGUAGES CXX C)


find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
if (NOT TARGET assimp)
    message(STATUS "Ready to Compile assimp")
    option(BUILD_SHARED_LIBS "" OFF)
    option(ASSIMP_INSTALL "" OFF)
    add_subdirectory(3rdparty/assimp)
endif()

add_library(learnopengl src/glad.c)
target_include_directories(learnopengl PUBLIC include)
target_link_libraries(learnopengl PUBLIC OpenGL::GL glfw)
target_compile_definitions(learnopengl PUBLIC RESOURCE_DIR="${CMAKE_SOURCE_DIR}/resource")

add_subdirectory(src/minimal)
add_subdirectory(src/directional_shadow_mapping)
add_subdirectory(src/point_shadow)
