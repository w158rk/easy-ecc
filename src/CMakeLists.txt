
aux_source_directory(. TEMP_SRC)
SET (MY_SRC ${TEMP_SRC})
aux_source_directory(./ecc/avr TEMP_SRC)
SET (MY_SRC ${TEMP_SRC})
aux_source_directory(./point/avr TEMP_SRC)
SET (MY_SRC ${TEMP_SRC})
aux_source_directory(./field/avr TEMP_SRC)
SET (MY_SRC ${TEMP_SRC})
aux_source_directory(./hash TEMP_SRC)
SET (MY_SRC ${TEMP_SRC})


if(CMAKE_BUILD_TYPE MATCHES AVR)
    generate_arduino_library(ecc 
        SRCS    ${MY_SRC}
        BOARD   mega2560
    )
else(CMAKE_BUILD_TYPE MATCHES AVR) 
    add_library(ecc ${MY_SRC})
endif(CMAKE_BUILD_TYPE MATCHES AVR)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(ecc PUBLIC DEBUG)
endif(CMAKE_BUILD_TYPE MATCHES Debug)
