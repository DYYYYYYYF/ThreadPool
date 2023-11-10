project(demos)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(GLOB_RECURSE demo_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" 
	"${CMAKE_CURRENT_SOURCE_DIR}/no_thread_pool/main.cpp")

foreach(demo_file IN LISTS demo_files)

	message("-- Generating ${demo_file}")

	get_filename_component(mainname ${demo_file} NAME_WE)
	add_executable(${mainname} ${demo_file})
	target_link_libraries(${mainname} udMutipleThread)

	message("-- Generate ${mainname} to ${CMAKE_CURRENT_SOURCE_DIR}/bin successful")

endforeach()

