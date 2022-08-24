cmake_minimum_required(VERSION 3.23)
project(yxdb_odbc C)

add_subdirectory(lib)

set(CMAKE_C_STANDARD 17)

add_executable(yxdb_odbc main.c)