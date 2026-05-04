cmake_minimum_required(VERSION 3.28)

project(
    "graphviz-demo"
    DESCRIPTION "Graphviz demo"
    LANGUAGES C
)

# global debug postfix for libraries (executables still need to set it)
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Filename postfix for libraries under DEBUG configuration")

# here it is a top-level project, so CMAKE_PROJECT_NAME is fine
add_executable(${CMAKE_PROJECT_NAME})

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        src/main.c
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GRAPHVIZ REQUIRED
    libcdt
    libcgraph
    libgvc
)

message(STATUS "GRAPHVIZ_INCLUDE_DIRS: ${GRAPHVIZ_INCLUDE_DIRS}")
message(STATUS "GRAPHVIZ_LIBRARIES: ${GRAPHVIZ_LIBRARIES}")

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${GRAPHVIZ_INCLUDE_DIRS}
)
target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${GRAPHVIZ_LIBRARIES}
)

include(GNUInstallDirs)

install(TARGETS ${CMAKE_PROJECT_NAME})
