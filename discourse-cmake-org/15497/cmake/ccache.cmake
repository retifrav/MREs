# based on Appendix A from the "Professional CMake" book by Craig Scott
function(useCompilerCache)
    if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # NOT PROJECT_IS_TOP_LEVEL
        message(DEBUG "This is not a top-level project, won't enable ccache")
        return()
    endif()

    find_program(CCACHE_EXECUTABLE ccache)
    if(NOT CCACHE_EXECUTABLE)
        message(
            WARNING
                "Project build is configured to use ccache, "
                "but ccache executable was not found in PATH"
        )
        return()
    endif()

    message(DEBUG "Found ccache: ${CCACHE_EXECUTABLE}")

    # ccache 4.7.4 manual says that `-fno-pch-timestamp` is required for Clang
    foreach(lang IN ITEMS C CXX) # OBJC OBJCXX
        if(
            CMAKE_${lang}_COMPILER_ID MATCHES "Clang"
            #AND # should these apply to clang-cl too?
            #NOT CMAKE_${lang}_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"
        )
            add_compile_options(
                "$<$<COMPILE_LANGUAGE:${lang}>:SHELL:-Xclang -fno-pch-timestamp>"
            )
        endif()
    endforeach()

    if(MSVC)
        # ccache does not work when building with `/Zi` (debug information in separate PDB files),
        # so you would need to use `/Z7` (debug information directly in object files)
        #
        # (skipping that part)
    endif()

    # does it override the `sloppiness` from `/path/to/.config/ccache/ccache.conf` or does it "merge" with it?
    set(CCACHE_ENV "CCACHE_SLOPPINESS=pch_defines,time_macros" # also maybe add `locale` to the list?
        CACHE STRING "Environment variables for ccache"
    )

    # generator-specific rituals

    if(CMAKE_GENERATOR MATCHES "Visual Studio")

        message(DEBUG "Compiler and generator variables so far:")
        list(APPEND CMAKE_MESSAGE_INDENT "- ")
        #
        message(DEBUG "CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
        message(DEBUG "CMAKE_C_COMPILER_FRONTEND_VARIANT: ${CMAKE_C_COMPILER_FRONTEND_VARIANT}")
        message(DEBUG "CMAKE_C_SIMULATE_ID: ${CMAKE_C_SIMULATE_ID}")
        #
        message(DEBUG "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
        message(DEBUG "CMAKE_CXX_COMPILER_FRONTEND_VARIANT: ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}")
        message(DEBUG "CMAKE_CXX_SIMULATE_ID: ${CMAKE_CXX_SIMULATE_ID}")
        #
        message(DEBUG "CMAKE_GENERATOR_TOOLSET: ${CMAKE_GENERATOR_TOOLSET}")
        message(DEBUG "CMAKE_VS_PLATFORM_TOOLSET: ${CMAKE_VS_PLATFORM_TOOLSET}")
        #
        list(POP_BACK CMAKE_MESSAGE_INDENT)

        cmake_path(NATIVE_PATH CCACHE_EXECUTABLE CCACHE_EXE)
        list(JOIN CCACHE_ENV "\nset " SET_ENV)
        if(NOT SET_ENV STREQUAL "")
            string(PREPEND SET_ENV "set ")
        endif()

        set(THIS_IS_CLANG_CL NO)

        # either C or CXX has to be enabled
        get_property(langs GLOBAL PROPERTY ENABLED_LANGUAGES)
        if(CXX IN_LIST langs)
            if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC") # CMAKE_GENERATOR_TOOLSET STREQUAL "ClangCL"
                set(THIS_IS_CLANG_CL YES)
            endif()
            set(compiler "${CMAKE_CXX_COMPILER}")
        else()
            if(CMAKE_C_COMPILER_ID STREQUAL "Clang" AND CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC") # CMAKE_GENERATOR_TOOLSET STREQUAL "ClangCL"
                set(THIS_IS_CLANG_CL YES)
            endif()
            set(compiler "${CMAKE_C_COMPILER}")
        endif()
        file(WRITE ${CMAKE_BINARY_DIR}/launch-cl.cmd
            "@echo off\n"
            "${SET_ENV}\n"
            "\"${CCACHE_EXE}\" \"${compiler}\" %*\n"
        )

        if(THIS_IS_CLANG_CL) # CMAKE_GENERATOR_TOOLSET STREQUAL "ClangCL"
            message(
                WARNING
                    "Trying to use ccache with Visual Studio generator and ClangCL toolset "
                    "will likely fail, because MSBuild(?) ignores(?) CLToolExe property in *.vcxproj "
                    "files and instead it always tries to use hardcoded (somewhere?) clang-cl.exe, "
                    "while still respecting the CLToolPath property, which results in "
                    "an invalid path. But even if you drop CLToolPath from CMAKE_VS_GLOBALS, "
                    "the main problem remains - ignoring CLToolExe property means that it will "
                    "never call launch-cl.cmd (generated script that enables ccache), so that "
                    "needs to be enforced with -- /p:CLToolExe=launch-cl.cmd, because apparently "
                    "it does not ignore the inline CLI arguments (adding "
                    "/p:CLToolPath=c:/path/to/project/build should not be required, "
                    "since it does get respected in *.vcxproj files, so don't drop it from CMAKE_VS_GLOBALS). "
                    "Or, perhaps, you could try using Ninja generator instead, but then you'll need to provide "
                    "-DCMAKE_{C,CXX}_COMPILER:PATH=c:/path/to/vs/VC/Tools/Llvm/x64/bin/clang-cl.exe "
                    "and unset the ClangCL toolset."
                )
        endif()

        list(FILTER CMAKE_VS_GLOBALS EXCLUDE REGEX "^(CLTool(Path|Exe)|TrackFileAccess)=.*$")
        list(APPEND CMAKE_VS_GLOBALS
            CLToolPath=${CMAKE_BINARY_DIR}
            CLToolExe=launch-cl.cmd
            TrackFileAccess=false
        )
        # forces the use of the Multi-ToolTask scheduler for a more(?) efficient CPU usage
        if(NOT CMAKE_VS_GLOBALS MATCHES "(^|;)UseMultiToolTask=")
            list(APPEND CMAKE_VS_GLOBALS UseMultiToolTask=true)
        endif()
        if(NOT CMAKE_VS_GLOBALS MATCHES "(^|;)EnforceProcessCountAcrossBuilds=")
            list(APPEND CMAKE_VS_GLOBALS EnforceProcessCountAcrossBuilds=true)
        endif()
        set(CMAKE_VS_GLOBALS "${CMAKE_VS_GLOBALS}" PARENT_SCOPE)

    else()
        message(
            WARNING
                "Current CMake generator is ${CMAKE_GENERATOR}, but this example is specifically "
                "about Visual Studio, so no rituals are performed for any other generator "
                "(meaning that ccache will likely not get enabled in this configuration)"
            )
    endif()
endfunction()
