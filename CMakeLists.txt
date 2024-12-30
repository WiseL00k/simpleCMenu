cmake_minimum_required(VERSION 3.10)

project(simpleCMenu)

set(CMAKE_CXX_STANDARD 11)

include_directories(./inc)

aux_source_directory(./src SRC)

set(EXECUTABLE_OUTPUT_PATH ./)

add_executable(simpleCMenu ${SRC})
