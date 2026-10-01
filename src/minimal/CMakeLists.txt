set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(minimal main.cpp)
target_link_libraries(minimal PRIVATE learnopengl)
