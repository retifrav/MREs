cmake_minimum_required(VERSION 3.28)

project(sdl-example
    LANGUAGES CXX
)

option(USING_PACKAGE_MANAGER "Using a package manager for resolving dependencies" 0)

if(
    NOT USING_PACKAGE_MANAGER
    AND
    NOT DEFINED SDL2_DIR
)
    message(WARNING
        "You have neither set to use a package manager with `USING_PACKAGE_MANAGER` "
        "nor provided a value for `SDL2_DIR` to find a pre-built SDL package; "
        "will fallback to finding SDL in the system environment (CMAKE_PREFIX_PATH)"
    )
endif()

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
    #find_package(X11 REQUIRED)

    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
            #${CMAKE_THREAD_LIBS_INIT}
            ${OPENGL_opengl_LIBRARY}
            #${X11_LIBRARIES}
            #${CMAKE_DL_LIBS}
    )
else()
    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
            ${OPENGL_gl_LIBRARY}
    )
endif()

message(DEBUG "Looking for SDL2...")
list(APPEND CMAKE_MESSAGE_INDENT "- ")
# package name might have a different case (GitHub releases have it lower-cased),
# but CMake will probe all the variants anyway, so there is no need to try both
find_package(SDL2 CONFIG REQUIRED)
if(SDL2_FOUND) # redundant check, because it was found with REQUIRED
    message(DEBUG "Found SDL2 here: ${SDL2_DIR}")
    message(DEBUG "SDL2 target(s): ${SDL2_LIBRARIES}")
endif()
list(POP_BACK CMAKE_MESSAGE_INDENT)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
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
#
# if SDL is built as a SHARED library (DLL), then you might want to automate its installation too,
# but if it is a STATIC library, then TARGET_RUNTIME_DLLS is likely to be empty,
# and so the following `add_custom_command()` will fail to execute
#
#if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#    if(NOT EXISTS "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
#        file(MAKE_DIRECTORY "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
#    endif()
#    add_custom_command(
#        TARGET ${CMAKE_PROJECT_NAME}
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}>
#            ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
#        COMMAND_EXPAND_LISTS
#        COMMENT "Copying runtime dependencies (DLLs)"
#    )
#endif()
