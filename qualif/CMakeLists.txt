cmake_minimum_required(VERSION 2.8)
project (hashcode)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost COMPONENTS
	system
	thread
	program_options)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g -O0")
endif()

set(SOURCES main.cpp stuff.h)
add_executable(hashcode ${SOURCES})

target_link_libraries(hashcode ${Boost_LIBRARIES})
