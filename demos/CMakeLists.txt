project(demos)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

file(GLOB_RECURSE demo_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" 
	"${CMAKE_CURRENT_SOURCE_DIR}/no_thread_pool/no_thread_pool_test.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/thread_pool/thread_pool_test.cpp")

foreach(demo_file IN LISTS demo_files)

	message("-- Generating ${demo_file}")

	get_filename_component(mainname ${demo_file} NAME_WE)
	add_executable(${mainname} ${demo_file})

	message("-- Generate ${mainname} to ${CMAKE_CURRENT_SOURCE_DIR}/bin successful")

endforeach()

