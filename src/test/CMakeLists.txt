link_directories(${CMAKE_BINARY_DIR}/)

include_directories (${PROJECT_INCLUDE_DIR} ../)

# add the executable
add_executable (keytest key.c)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(keytest PUBLIC DEBUG)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

target_link_libraries (keytest ecc)
