
cmake_minimum_required (VERSION 2.6)
SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)#设置C++编译器

project (EASYECC)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE MATCHES Analyze)
    set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g -pg)
    SET(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -pg)
    SET(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} -pg)
    add_compile_options(${CMAKE_CXX_FLAGS})
endif(CMAKE_BUILD_TYPE MATCHES Analyze)

include_directories(${CMAKE_SOURCE_DIR}/src/include)
link_directories(${CMAKE_SOURCE_DIR}/src/lib)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/test)
