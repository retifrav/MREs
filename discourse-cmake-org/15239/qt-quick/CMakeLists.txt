cmake_minimum_required(VERSION 3.22)

project("some"
    DESCRIPTION "Some application using Qt Quick 5.x"
    VERSION 1.2.3
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# quite annoying, but it has to be all capital `QT` in the first find_package()
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Quick Qml QmlWorkerScript)
find_package(Qt5 COMPONENTS Quick Qml QmlWorkerScript REQUIRED)

if (Qt5_FOUND)
    get_filename_component(QT_HOME "${Qt5_DIR}/../../.." ABSOLUTE)
    message(STATUS "Found Qt ${QT_VERSION}: ${QT_HOME}")
else()
    message(FATAL_ERROR "Couldn't find Qt")
endif()

#set(CMAKE_AUTOUIC ON) # not needed in a Qt Quick application (unless it has a UI-form?)
set(CMAKE_AUTOMOC ON) # otherwise `Undefined symbols `Backend::qt_metacall(QMetaObject::Call, int, void**)` from the `qmlRegisterType`

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_executable(${CMAKE_PROJECT_NAME} WIN32)
else()
    add_executable(${CMAKE_PROJECT_NAME})
endif()

get_target_property(QT_TARGET_TYPE Qt5::Core TYPE)
if(${QT_TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
    message(STATUS "This Qt is static")

    if(MINGW)
        # without it there will be runtime error about missing MinGW runtime,
        # even if you configured Qt with -static-runtime
        #
        # probably there is a better way, but that works too
        set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
        # and without these there will be runtime errors about missing
        # zstd and zlib DLLs, and if you remove QtCore from here,
        # it will still complain about missing zstd despite having -lzstd
        target_link_libraries(${CMAKE_PROJECT_NAME}
            PRIVATE
                Qt5::Core
                -lz
                -lzstd
        )
    endif()

    # from 5.14 there is a standard function for this
    set(QT_VERSION_WITH_QML_IMPORTS 5.14)
    if(
        ${QT_VERSION} VERSION_EQUAL ${QT_VERSION_WITH_QML_IMPORTS}
        OR
        ${QT_VERSION} VERSION_GREATER ${QT_VERSION_WITH_QML_IMPORTS}
    )
        message(STATUS "This Qt version (${QT_VERSION}) is newer than ${QT_VERSION_WITH_QML_IMPORTS}")
        find_package(Qt5QmlImportScanner REQUIRED)
        if(NOT Qt5QmlImportScanner_FOUND)
            message(FATAL_ERROR "Could not find the Qt5QmlImportScanner module")
        endif()
        qt5_import_qml_plugins(${CMAKE_PROJECT_NAME})
    else() # use a custom module
        message(STATUS "This Qt version (${QT_VERSION}) is older than ${QT_VERSION_WITH_QML_IMPORTS}")
        include(QtStaticPlugins)
        qt_generate_qml_plugin_import(${CMAKE_PROJECT_NAME}
            QML_DIR "${QT_HOME}/qml"
            QML_SRC "${CMAKE_CURRENT_SOURCE_DIR}"
            OUTPUT "${CMAKE_PROJECT_NAME}_qml_plugin_import.cpp"
            OUTPUT_DIR "${PROJECT_BINARY_DIR}"
            #VERBOSE
        )
    endif()

    # this target needs to be linked to as well, otherwise `undefined reference to QQmlWorkerScriptModule::registerQuickTypes()`
    if(TARGET Qt5::QmlWorkerScript)
        target_link_libraries(${CMAKE_PROJECT_NAME}
            PRIVATE
                Qt5::QmlWorkerScript
        )
    endif()
else()
    message(STATUS "This Qt is ${QT_TARGET_TYPE}")
endif()

set(sources
    backend.cpp
    main.cpp
)

qt_add_resources(sources qml.qrc) # or add it to the `sources` list and do `set(CMAKE_AUTORCC ON)`

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${sources}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        Qt5::Quick
        Qt5::Qml
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
