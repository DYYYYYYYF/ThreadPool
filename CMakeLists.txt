cmake_minimum_required(VERSION 3.0.0)
project(udMutipleThread VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${APPLE})
	set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/library/MacOS)
elseif(${WIN32})
	set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/library/Windows)
endif()

include_directories(./include)
aux_source_directory(./src RESOURCE_DIR)

add_subdirectory(demos)

add_library(udMutipleThread ${RESOURCE_DIR})

