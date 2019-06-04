cmake_minimum_required(VERSION 2.8)

project("forth")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(forth_version 0.2)

add_subdirectory(source)


