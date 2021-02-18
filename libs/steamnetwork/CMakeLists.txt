project(SteamNetwork)

add_library(${PROJECT_NAME} STATIC src/steamnetwork.cpp)

target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/lib/GameNetworkingSockets_s.lib)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        )