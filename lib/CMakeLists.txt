cmake_minimum_required(VERSION 3.23)
project(lib C)

set(CMAKE_C_STANDARD 17)

add_library(lib odbc.c lzf.c)