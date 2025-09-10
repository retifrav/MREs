## Several variants of a shared library with the same interface

<https://discourse.cmake.org/t/multiple-shared-libs-alternative-implementations-of-common-interface-minimal-cmake-c-example/15161/>

### Building

Dependency (*one of the variants*):

``` sh
$ cd /path/to/dpndnc/
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=YES \
    -DCMAKE_INSTALL_PREFIX="../install" \
    -DDPNDNC_BUILD_VARIANT_1=YES \
    -DDPNDNC_BUILD_VARIANT_2=NO \
    ..
$ cmake --build . --target install
```

Tool:

``` sh
$ cd /path/to/tl/
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="../install" \
    -DCMAKE_PREFIX_PATH="/path/to/dpndnc/install" \
    ..
$ cmake --build . --target install
```

### Running

When dependency variant #1 was built and installed:

``` sh
DYLD_LIBRARY_PATH=../../dpndnc/install/lib ../install/bin/some-tool
Base application message

Dependency:
ololo, some string from the dependency alternative #1
```

When dependency variant #2 was built and installed (*obviously, there is no need to re-build the tool for that*):

``` sh
$ DYLD_LIBRARY_PATH=../../dpndnc/install/lib ../install/bin/some-tool
Base application message

Dependency:
ololo, another string from the dependency alternative #2
```

The `DYLD_LIBRARY_PATH` thing is for Mac OS, on Linux it would be `LD_LIBRARY_PATH`, and on Windows you just need to copy the DLL alongside the executable.
