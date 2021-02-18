cmake_minimum_required(VERSION 3.7)
project(F4MP_Client)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} CMakeModules/)

set(EXECUTABLE F4MP_Client)
set(SRC_DIR ./src)
set(INCLUDE_DIR ./include)

add_definitions(-D_RESOURCES="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
message(${CMAKE_CURRENT_SOURCE_DIR}/resources)

set(THREADS_PREFER_PTHREAD_FLAD ON)
find_package(Threads REQUIRED)

add_subdirectory(libs/imgui)
add_subdirectory(libs/spdlog)
add_subdirectory(libs/steam)
add_subdirectory(libs/steamnetwork)
add_subdirectory(libs/hooks)

include_directories(${EXECUTABLE}
	${INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB SourceFiles 
	${SRC_DIR}/*
	${SRC_DIR}/Utils/*
	${SRC_DIR}/ThirdParty/*
	${SRC_DIR}/Hooks/*
)


add_library(${EXECUTABLE} SHARED ${SourceFiles})

set_target_properties(${EXECUTABLE} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS ON
)

target_link_libraries(${EXECUTABLE}
	${CMAKE_DL_LIBS}
	${CMAKE_CURRENT_SOURCE_DIR}/lib/detours.lib
	d3d11.lib
	dxgi.dll
	imgui
	spdlog::spdlog_header_only
	Steam
	SteamNetwork
	Hooks
)