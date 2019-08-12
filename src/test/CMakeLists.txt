link_directories(${CMAKE_BINARY_DIR}/)

include_directories (${PROJECT_INCLUDE_DIR} ../)

# add the executable


# add_executable (keytest key.c)
generate_arduino_firmware(keytest 
    SRCS    key.cpp
    PORT    /dev/ttyACM0
    LIBS    ecc    
    BOARD   mega2560 
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(keytest PUBLIC DEBUG)
endif(CMAKE_BUILD_TYPE MATCHES Debug)
if(CMAKE_BUILD_TYPE MATCHES Analyze)
    target_compile_definitions(keytest PUBLIC ANALYZE)
endif(CMAKE_BUILD_TYPE MATCHES Analyze)
