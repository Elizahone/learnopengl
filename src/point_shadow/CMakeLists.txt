set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(point_shadow main.cpp)
target_link_libraries(point_shadow PRIVATE learnopengl)
target_compile_definitions(point_shadow PRIVATE
    PWD="${CMAKE_CURRENT_SOURCE_DIR}")
