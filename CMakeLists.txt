#=======================================================#
# Author    :   Wang Ruikai                             #
# Date      :   12.08.2019                              #
#                                                       #
# Description : Arduino easy-ecc                        #
#                                                       #
#=======================================================#    


if(CMAKE_BUILD_TYPE MATCHES Release-avr-[a-z0-9-]*)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/src/cmake/ArduinoToolchain.cmake)
else(CMAKE_BUILD_TYPE MATCHES Release-avr-[a-z0-9-]*)
    set(CMAKE_C_COMPILER   gcc)
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif(CMAKE_BUILD_TYPE MATCHES Release-avr-[a-z0-9-]*)

cmake_minimum_required (VERSION 2.8)
project (EASYECC C CXX)


if(NOT CMAKE_BUILD_TYPE)
    message("Choose From the following to specify a build type")
    message("1. \t Release")
    message("2. \t Debug")
    message("3. \t Analyze")
    message("4. \t Release-avr")
    message("use default Release")
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
