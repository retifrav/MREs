cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
message(STATUS "Qt6_FOUND: ${Qt6_FOUND}")

include(FetchContent)

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
set(CMAKE_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Include)
set(CMAKE_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Lib/win64)

message(STATUS "The CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")
message(STATUS "CMAKE_LIBRARY_PATH: ${CMAKE_LIBRARY_PATH}")

cmake_path(APPEND FULL_LIB_NAME "${CMAKE_LIBRARY_PATH}" "R3DSDK-2017MD.lib")
message("Full name: ${FULL_LIB_NAME}")


#qt_standard_project_setup()

add_executable(helloworld
    main.cpp
    R3DSDK.h
    R3DSDKDefinitions.h
)

target_include_directories(helloworld PUBLIC ${CMAKE_INCLUDE_PATH})

target_link_libraries(helloworld PUBLIC Qt6::Core ${FULL_LIB_NAME})
message(STATUS "End of CMAKELIST.TXT!")
