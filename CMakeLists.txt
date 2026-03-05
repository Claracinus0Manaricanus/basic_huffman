cmake_minimum_required(VERSION 4.1.0)

project(cm_basic_huffman)

include_directories(./include)

add_library(libcm_simple_huffman
	"src/basic_huffman.c"
)

if(CM_TEST)
	add_executable(test tests/main.c)
endif()
