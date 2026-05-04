# Finding and linking Graphviz with pkg-config

<https://discourse.cmake.org/t/how-can-i-link-the-graphviz-library-into-my-project-which-is-a-plugin-library-for-the-kate-texteditor/15639/>

## Installing Graphviz

On Arch Linux:

``` sh
$ sudo pacman -S graphviz
```

There is no CMake config, so we'll have to use pkg-config:

``` sh
$ pacman -Ql graphviz | grep -i cmake

$ pacman -Ql graphviz | grep -i pc
graphviz /usr/lib/pkgconfig/libcdt.pc
graphviz /usr/lib/pkgconfig/libcgraph.pc
graphviz /usr/lib/pkgconfig/libgvc.pc
graphviz /usr/lib/pkgconfig/libgvpr.pc
graphviz /usr/lib/pkgconfig/libpathplan.pc
graphviz /usr/lib/pkgconfig/libxdot.pc
```

## Building and running

``` sh
$ cd /path/to/15639/
$ mkdir build && cd $_

$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="../install" \
    ..
$ cmake --build . --target install

$ ../install/bin/graphviz-demo -K dot
```
