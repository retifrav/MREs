## Resolving png and zlib dependencies

<https://cyberforum.ru/cpp-cross-platform/thread3213759.html>

### Building

#### Without package manager

Using [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html), so without a proper package manager:

``` sh
$ cd /path/to/3213759/
 
$ cmake --preset not-using-package-manager
$ cmake --build --preset not-using-package-manager
 
$ cd ./install/not-using-package-manager/bin/
$ ./some
1025x289
```

#### With vcpkg

Using [vcpkg](https://vcpkg.io/) and building for Windows:

``` sh
$ cd /path/to/3213759/
 
$ cmake --preset using-vcpkg-windows-static
$ cmake --build --preset using-vcpkg-windows-static
 
$ cd ./install/using-vcpkg-windows-static/bin/
$ ./some.exe
1025x289
```

#### With Conan

Using [Conan](https://conan.io/) (*specifically, version 2.x*) and building for default/auto-detected Conan profile:

``` sh
$ cd /path/to/3213759/

$ conan install . \
    -r YOUR-CONAN-REMOTE \
    -s build_type=Release \
    --build missing \
    --output-folder ./build/using-conan-default \
    -c tools.cmake.cmaketoolchain:user_presets=""

$ cmake --preset using-conan-default
$ cmake --build --preset using-conan-default

$ cd ./install/using-conan-default/bin/
$ ./some
1025x289
```

You'll need to adjust `requires` section in the `conanfile.txt`, because `decovar/public` is specific to a non-public Conan remote I am using. Also, instead of `YOUR-CONAN-REMOTE` there should be your own remote or maybe Conan Center, but then you will likely need to adjust `#include` statements for zlib and png public headers in the `main.cpp`.

Also, if you provide that `-c` option for `conan install` to set the empty value for `user_presets`, then you don't actually need to keep [that](https://github.com/conan-io/cmake-conan/issues/484#issuecomment-3958056927) dummy `CMakeUserPresets.json`.
