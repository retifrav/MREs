cmake_minimum_required(VERSION 3.22)

set(DPNDNC_PROJECT_NAMESPACE "dpndnc")
project(
    "some"
    DESCRIPTION "Some dependency in two variants"
    VERSION 1.2.3
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# global debug postfix for libraries (executables still need to set it)
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Filename postfix for libraries under DEBUG configuration")

option(DPNDNC_BUILD_VARIANT_1 "Build the variant #1 of the library" YES)
option(DPNDNC_BUILD_VARIANT_2 "Build the variant #2 of the library" NO)

if(NOT DPNDNC_BUILD_VARIANT_1 AND NOT DPNDNC_BUILD_VARIANT_2)
    message(FATAL_ERROR "You haven't chosen any variants of the dependency, so nothing will be built")
endif()

if(DPNDNC_BUILD_VARIANT_1 AND DPNDNC_BUILD_VARIANT_2)
    message(FATAL_ERROR "You cannot choose both variants of the dependency, as they install into the same prefix")
endif()

add_subdirectory(libraries)
