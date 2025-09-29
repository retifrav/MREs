## Resolving png and zlib dependencies

<https://cyberforum.ru/cpp-cross-platform/thread3213759.html>

### Building

#### Without package manager

Without a package manager, using [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html):

``` sh
$ cd /path/to/3213759/
 
$ cmake --preset not-using-package-manager
$ cmake --build --preset not-using-package-manager
 
$ cd ./install/not-using-package-manager/bin/
$ ./some.exe
1025x289
```

#### With vcpkg

Using [vcpkg](https://vcpkg.io/):

``` sh
$ cd /path/to/3213759/
 
$ cmake --preset using-vcpkg-windows-static
$ cmake --build --preset using-vcpkg-windows-static
 
$ cd ./install/using-vcpkg-windows-static/bin/
$ ./some.exe
1025x289
```

