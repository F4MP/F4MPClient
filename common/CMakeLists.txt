set(PROJ Common)

set(SRC_DIR ./src)
set(INCLUDE_DIR ./include)

file(GLOB SOURCE_FILES
        ${SRC_DIR}/*
        )

add_library (${PROJ} STATIC  ${SOURCE_FILES})

include_directories(${PROJ} ${INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories (${PROJ} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/*)