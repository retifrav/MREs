cmake_minimum_required(VERSION 3.22)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
message(STATUS "Qt6_FOUND: ${Qt6_FOUND}")

#qt_standard_project_setup()

add_executable(${PROJECT_NAME}
    main.cpp
)

find_library(R3DSDK_LIBRARY
    NAMES R3DSDK-2017MD
    PATHS "${CMAKE_CURRENT_SOURCE_DIR}/Lib/win64"
    PATH_SUFFIXES "lib"
    #NO_CMAKE_FIND_ROOT_PATH
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
    #PUBLIC
    #    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    #    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        ${R3DSDK_LIBRARY}
)
