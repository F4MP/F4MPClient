set(PROJ Common)

set(SRC_DIR ./src)

file(GLOB SOURCE_FILES
        ${SRC_DIR}/*
        )

add_library (${PROJ} ${SOURCE_FILES})

target_include_directories (${PROJ} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/*)