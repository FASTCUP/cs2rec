cmake_minimum_required(VERSION 3.20)

project(recorder)

add_library(cs2rec SHARED)
add_subdirectory(src)
include_directories(deps)
include_directories(src)

# Link cdl86
target_sources(cs2rec PRIVATE deps/cdl86/cdl.c)
include_directories(deps/cdl86)

set(SF_DEBUG 0 CACHE STRING "Prevent execution until the sf_debug command is used in-game")
if (${SF_DEBUG})
    add_compile_definitions(SF_DEBUG)
endif()