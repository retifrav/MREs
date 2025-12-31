# Finding SDL

<https://discourse.cmake.org/t/find-package-working-at-main-level-failing-on-dependencies/15414>

Examples of finding SDL and building a simple application. SDL can be either downloaded directly as a pre-built package or resolved with vcpkg.

## Building

### Downloading pre-built package

``` sh
$ cd /path/to/15414/
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="../install" \
    -DSDL_DIR="/path/to/sdl" \
    ..
$ cmake --build . --target install
$ ../install/bin/sdl-example
```

### Resolving with vcpkg

``` sh
$ cd /path/to/15414/
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="../install" \
    -DUSING_PACKAGE_MANAGER=1 \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
    ..
$ cmake --build . --target install
$ ../install/bin/sdl-example
```
