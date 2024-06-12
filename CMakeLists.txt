cmake_minimum_required(VERSION 3.20)

project(recorder)

add_library(recorder SHARED)
add_subdirectory(src)
include_directories(deps)
include_directories(src)

# Link cdl86
target_sources(recorder PRIVATE deps/cdl86/cdl.c)
include_directories(deps/cdl86)