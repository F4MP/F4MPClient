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

add_subdirectory (libs/imgui)

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

add_subdirectory(Common)

add_library(${EXECUTABLE} SHARED ${SourceFiles})

set_target_properties(${EXECUTABLE} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS ON
)

target_link_libraries(${EXECUTABLE}
	${CMAKE_DL_LIBS}
	${CMAKE_CURRENT_SOURCE_DIR}/lib/steam_api64.lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/detours.lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/spdlogd.lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/GameNetworkingSockets_s.lib
	Common
	d3d11.lib
	dxgi.dll
	f4mp::imgui
)