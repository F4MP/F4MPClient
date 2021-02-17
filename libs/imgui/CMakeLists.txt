project (imgui)

file(GLOB SOURCE "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)