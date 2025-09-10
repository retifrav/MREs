cmake_minimum_required(VERSION 3.22)

project(
    "some-tool"
    DESCRIPTION "A CLI tool that does something"
    VERSION 0.9.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# global debug postfix for libraries (executables still need to set it)
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Filename postfix for libraries under DEBUG configuration")

# here it's a top-level project, so CMAKE_PROJECT_NAME is fine
add_executable(${CMAKE_PROJECT_NAME})

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        src/main.cpp
)

find_package(some CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        dpndnc::some
)

# ---

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME})
