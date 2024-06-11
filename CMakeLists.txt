cmake_minimum_required(VERSION 3.20)

project(recorder)

add_library(recorder SHARED)
add_subdirectory(src)
include_directories(deps)
include_directories(src)