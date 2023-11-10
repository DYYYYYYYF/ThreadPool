cmake_minimum_required(VERSION 3.0.0)
project(udMutipleThread VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/library)

include_directories(./include)
aux_source_directory(./src RESOURCE_DIR)

add_library(udMutipleThread ${RESOURCE_DIR})

add_executable(test_project main.cpp)

target_link_libraries(test_project udMutipleThread)