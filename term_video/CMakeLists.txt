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
target_link_libraries(${target_name} fmt)
target_compile_definitions(${target_name} PUBLIC NOMINMAX)

target_include_directories(${target_name} PUBLIC ${target_src_root})

add_custom_command(TARGET ${target_name}
	POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
	${src_content_dir}
	$<TARGET_FILE_DIR:${target_name}>/content)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})