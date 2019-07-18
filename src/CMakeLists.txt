
aux_source_directory(./ TEMP_SRC)
SET (MY_SRC ${TEMP_SRC})
aux_source_directory(./ecc TEMP_SRC)
SET (MY_SRC ${TEMP_SRC})
aux_source_directory(./point TEMP_SRC)
SET (MY_SRC ${TEMP_SRC})

add_library(ecc ${MY_SRC})
