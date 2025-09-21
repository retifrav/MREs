message(STATUS "Resolving png with FetchContent...")

list(APPEND CMAKE_MESSAGE_INDENT "+ ")

if(DEFINED BUILD_SHARED_LIBS AND BUILD_SHARED_LIBS)
    set(PNG_SHARED 1 CACHE BOOL "" FORCE)
    set(PNG_STATIC 0 CACHE BOOL "" FORCE)
else()
    set(PNG_SHARED 0 CACHE BOOL "" FORCE)
    set(PNG_STATIC 1 CACHE BOOL "" FORCE)
endif()
set(PNG_TESTS 0 CACHE BOOL "" FORCE)
set(PNG_TOOLS 0 CACHE BOOL "" FORCE)
#set(PNG_EXECUTABLES 0 CACHE BOOL "" FORCE)

# the original png project is retarded too and requires patching as well
FetchContent_Declare(png
    GIT_REPOSITORY git@github.com:pnggroup/libpng.git # or https://github.com/pnggroup/libpng.git, if you don't use SSH
    GIT_TAG ed217e3e601d8e462f7fd1e04bed43ac42212429 # v1.6.43
    PATCH_COMMAND ${GIT} apply "${CMAKE_CURRENT_SOURCE_DIR}/001-single-unsuffixed-target-dependencies-installation.patch"
    UPDATE_DISCONNECTED YES # prevents patching after it has already been patched
)
FetchContent_MakeAvailable(png)

list(POP_BACK CMAKE_MESSAGE_INDENT)
