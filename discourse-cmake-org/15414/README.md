# Finding SDL

<https://discourse.cmake.org/t/find-package-working-at-main-level-failing-on-dependencies/15414>

Example of finding SDL and building a simple application with it. The SDL can be either downloaded as a [pre-built package](https://github.com/libsdl-org/SDL/releases) or resolved with [vcpkg](https://github.com/retifrav/vcpkg-registry/tree/92b76403bc3c83114504b930c860bcb420f9d52f/ports/sdl).

## Building

### Using a pre-built package

``` sh
$ cd /path/to/15414/
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="../install" \
    -DUSING_PACKAGE_MANAGER=0 \
    -DSDL2_DIR="/path/to/sdl/cmake" \
    --log-level debug \
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
    --log-level debug \
    ..
$ cmake --build . --target install
$ ../install/bin/sdl-example
```
