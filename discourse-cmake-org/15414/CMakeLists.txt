cmake_minimum_required(VERSION 3.28)

project(sdl-example
    LANGUAGES CXX
)

option(USING_PACKAGE_MANAGER "Using a package manager for resolving dependencies" 0)

set(REQUIRED_CPP_STANDARD 11 CACHE STRING "Required C++ standard")
if(DEFINED CMAKE_CXX_STANDARD)
    if(CMAKE_CXX_STANDARD LESS ${REQUIRED_CPP_STANDARD})
        message(FATAL_ERROR "This project requires at least C++${REQUIRED_CPP_STANDARD}")
    endif()
else()
    set(CMAKE_CXX_STANDARD ${REQUIRED_CPP_STANDARD})
endif()
set(CMAKE_CXX_STANDARD_REQUIRED YES)
if(NOT DEFINED CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS NO)
endif()

set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Filename postfix for libraries under DEBUG configuration")

set(sources
    main.cpp
)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${sources}
)

# vcpkg toolchain sets CMAKE_FIND_FRAMEWORK to LAST, and that leads to finding OpenGL from Homebrew,
# which then results in segmentation fault crash on application launch on Mac OS
set(CMAKE_FIND_FRAMEWORK_BACKUP ${CMAKE_FIND_FRAMEWORK})
if(
    DEFINED VCPKG_INSTALLED_DIR
    AND
    CMAKE_SYSTEM_NAME STREQUAL "Darwin"
)
    set(CMAKE_FIND_FRAMEWORK "FIRST")
endif()
find_package(OpenGL REQUIRED)
set(CMAKE_FIND_FRAMEWORK ${CMAKE_FIND_FRAMEWORK_BACKUP})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    #find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)

    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
            #${CMAKE_THREAD_LIBS_INIT}
            ${X11_LIBRARIES}
            ${CMAKE_DL_LIBS}
    )
endif()

# release packages from GitHub are lower-cased
if(USING_PACKAGE_MANAGER)
    find_package(SDL2 CONFIG REQUIRED)
else()
    find_package(sdl2 CONFIG REQUIRED)
endif()

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${OPENGL_gl_LIBRARY}
        SDL2::SDL2
        SDL2::SDL2main
)

if(USING_PACKAGE_MANAGER)
    target_compile_definitions(${CMAKE_PROJECT_NAME}
        PRIVATE
            USING_PACKAGE_MANAGER
    )
endif()

target_compile_features(${PROJECT_NAME}
    PRIVATE
        "cxx_std_${REQUIRED_CPP_STANDARD}"
)

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

include(GNUInstallDirs)

install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
