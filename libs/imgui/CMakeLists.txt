project (imgui)

file(GLOB SOURCE "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCE})
add_library(f4mp::imgui ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)