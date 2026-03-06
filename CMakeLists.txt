cmake_minimum_required(VERSION 4.1.0)

project(cm_basic_huffman)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(./include)

add_library(libcm_simple_huffman
	"src/basic_huffman.c"
)

if(CM_TEST)
	add_executable(test tests/main.c)
	target_link_libraries(test libcm_simple_huffman)
endif()
