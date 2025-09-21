message(STATUS "Resolving zlib with FetchContent...")

list(APPEND CMAKE_MESSAGE_INDENT "+ ")

set(ZLIB_BUILD_EXAMPLES 0 CACHE BOOL "" FORCE)

# the original zlib project is retarded, it is close to impossible to build it without patching
FetchContent_Declare(zlib
    GIT_REPOSITORY git@github.com:madler/zlib.git # or https://github.com/madler/zlib.git, if you don't use SSH
    GIT_TAG 51b7f2abdade71cd9bb0e7a373ef2610ec6f9daf # v1.3.1
    PATCH_COMMAND ${GIT} apply "${CMAKE_CURRENT_SOURCE_DIR}/001-single-target-and-installation.patch"
    UPDATE_DISCONNECTED YES # prevents patching after it has already been patched
    #FIND_PACKAGE_ARGS NAMES ZLIB
    OVERRIDE_FIND_PACKAGE # otherwise png (also resolved with FetchContent) won't find it
)
FetchContent_MakeAvailable(zlib)

list(POP_BACK CMAKE_MESSAGE_INDENT)
