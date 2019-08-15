link_directories(${CMAKE_BINARY_DIR}/)

include_directories (${PROJECT_INCLUDE_DIR} ../)

# add the executable


add_executable (keytest key.c)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(keytest PUBLIC DEBUG)
endif(CMAKE_BUILD_TYPE MATCHES Debug)
if(CMAKE_BUILD_TYPE MATCHES Analyze)
    target_compile_definitions(keytest PUBLIC ANALYZE)
endif(CMAKE_BUILD_TYPE MATCHES Analyze)

target_link_libraries (keytest ecc)

add_executable (pointtest point.c)
target_link_libraries (pointtest ecc)

add_executable (fieldtest field.c)
target_link_libraries (fieldtest ecc)

add_executable (hashtest hash.c)
target_link_libraries (hashtest ecc)
