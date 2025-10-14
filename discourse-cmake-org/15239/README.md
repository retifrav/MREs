# Finding Qt 5

<https://discourse.cmake.org/t/cmake-auto-moc-rcc-uic-executable-behavior-in-older-versions-of-cmake/15239>

Example of finding Qt 5 and building some simple applications.

## Building

``` sh
$ cd /path/to/15239/qt-project
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="../install" \
    -DCMAKE_PREFIX_PATH="/path/to/qt5" \
    ..
$ cmake --build . --target install
$ ../install/bin/some.app/Contents/MacOS/some
```
