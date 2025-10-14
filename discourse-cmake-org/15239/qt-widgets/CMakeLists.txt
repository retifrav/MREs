cmake_minimum_required(VERSION 3.22)

project("some"
    DESCRIPTION "Some application using Qt Widgets 5.x"
    VERSION 1.2.3
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# quite annoying, but it has to be all capital `QT` in the first find_package()
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(Qt5_FOUND)
    get_filename_component(QT_HOME "${Qt5_DIR}/../../.." ABSOLUTE)
    message(STATUS "Found Qt ${QT_VERSION}: ${QT_HOME}")
else()
    message(FATAL_ERROR "Couldn't find Qt")
endif()

# no qt_standard_project_setup() until Qt 6.3, so
set(CMAKE_AUTOUIC ON) # otherwise `ui_mainwindow.h file not found`
set(CMAKE_AUTOMOC ON) # otherwise `Undefined symbols vtable for MainWindow`
#
#set(CMAKE_AUTORCC ON) # if you are going to bundle .qrc

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_executable(${CMAKE_PROJECT_NAME} WIN32)
else()
    add_executable(${CMAKE_PROJECT_NAME})
endif()

set(sources
    main.cpp
    mainwindow.cpp
)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${sources}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        Qt5::Widgets
)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    #target_link_libraries(${CMAKE_PROJECT_NAME}
    #    PRIVATE
    #        "-framework Cocoa"
    #        "-framework IOKit"
    #        "-framework CoreVideo"
    #)

    set_target_properties(${CMAKE_PROJECT_NAME}
        PROPERTIES
            MACOSX_BUNDLE_GUI_IDENTIFIER "dev.decovar"
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION} # CFBundleLongVersionString, deprecated
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION} # CFBundleShortVersionString
    )
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)

    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
            ${CMAKE_THREAD_LIBS_INIT}
            ${X11_LIBRARIES}
            ${CMAKE_DL_LIBS}
    )
endif()

# --- installation

include(GNUInstallDirs)

set(EXECUTABLE_INSTALLATION_PATH ${CMAKE_INSTALL_BINDIR}/${CMAKE_PROJECT_NAME})

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    install(TARGETS ${CMAKE_PROJECT_NAME}
        BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
else()
    install(TARGETS ${CMAKE_PROJECT_NAME}
        RUNTIME DESTINATION ${EXECUTABLE_INSTALLATION_PATH}
    )
endif()
