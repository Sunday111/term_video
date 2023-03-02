cmake_minimum_required(VERSION 3.16)

include(generic_compile_options)

set(target_name term_video)
get_filename_component(target_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(target_src_root ${CMAKE_CURRENT_SOURCE_DIR}/code)
set(src_content_dir ${CMAKE_CURRENT_SOURCE_DIR}/content)

file(GLOB_RECURSE headers_list "${target_src_root}/*.hpp")
file(GLOB_RECURSE sources_list "${target_src_root}/*.cpp")

add_executable(${target_name}
	${headers_list}
	${sources_list})

set_generic_compile_options(${target_name} PUBLIC)
target_compile_definitions(${target_name} PUBLIC NOMINMAX)

add_custom_command(TARGET ${target_name}
	POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
	${src_content_dir}
	$<TARGET_FILE_DIR:${target_name}>/content)

find_package(OpenCV REQUIRED  COMPONENTS core highgui imgproc videoio)

#message(WARNING "OpenCV_LIBS: ${OpenCV_LIBS}")

# Add a custom command to copy the OpenCV dynamic libraries
add_custom_command(TARGET ${target_name} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:opencv_core>
		$<TARGET_FILE:opencv_highgui>
		$<TARGET_FILE:opencv_imgproc>
		$<TARGET_FILE:opencv_videoio>
		$<TARGET_FILE_DIR:${target_name}>
)
target_include_directories(${target_name} PUBLIC ${target_src_root} ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${target_name} PUBLIC fmt ${OpenCV_LIBS})
