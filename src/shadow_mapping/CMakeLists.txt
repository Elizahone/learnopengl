set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(shadow_mapping main.cpp ../glad.c)
target_link_libraries(shadow_mapping PRIVATE learnopengl)
target_compile_definitions(shadow_mapping PRIVATE
    PWD="${CMAKE_CURRENT_SOURCE_DIR}")
