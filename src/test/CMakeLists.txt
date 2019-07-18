link_directories(${CMAKE_BINARY_DIR}/)

include_directories (${PROJECT_INCLUDE_DIR} ../)

# add the executable
add_executable (keytest key.c)
target_link_libraries (keytest ecc)
