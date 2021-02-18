project(Steam)

add_library(${PROJECT_NAME} STATIC src/steam.cpp)

target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/lib/steam_api64.lib)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)