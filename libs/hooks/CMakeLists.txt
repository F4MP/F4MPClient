project(Hooks)

file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/lib/detours.lib)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)