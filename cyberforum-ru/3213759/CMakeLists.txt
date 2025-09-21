cmake_minimum_required(VERSION 3.22)

project(
    "some"
    DESCRIPTION "Something that uses png (and zlib)"
    VERSION 1.2.3
    LANGUAGES CXX
)

option(USING_PACKAGE_MANAGER "Using a proper package manager for resolving dependencies" YES)
option(INSTALL_DEPENDENCIES_TOO "Installing vcpkg-resolved dependencies" NO)

# global debug postfix for libraries (executables still need to set it)
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Filename postfix for libraries under DEBUG configuration")

if(NOT USING_PACKAGE_MANAGER)
    include(FetchContent)
    add_subdirectory(3rd-party)
endif()

# here it's a top-level project for an executable, so CMAKE_PROJECT_NAME is fine
add_executable(${CMAKE_PROJECT_NAME})

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        src/main.cpp
)

if(USING_PACKAGE_MANAGER)
    target_compile_definitions(${CMAKE_PROJECT_NAME}
        PRIVATE
            USING_PACKAGE_MANAGER
    )

    find_package(zlib CONFIG REQUIRED)
    find_package(png CONFIG REQUIRED)
else()
    # CMake config aren't(?) used in case of FetchContent, so
    target_include_directories(${CMAKE_PROJECT_NAME}
        PRIVATE
            ${png_SOURCE_DIR}
            ${png_BINARY_DIR}
    )
endif()

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        zlib
        png
)

include(GNUInstallDirs)

install(TARGETS ${CMAKE_PROJECT_NAME})
install(FILES
    resources/some.png
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(USING_PACKAGE_MANAGER AND INSTALL_DEPENDENCIES_TOO)
    install(DIRECTORY "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/"
        DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
endif()
