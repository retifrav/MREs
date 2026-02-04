# Using ccache with Visual Studio generator and ClangCL toolset

<https://discourse.cmake.org/t/using-ccache-with-visual-studio-generator-and-clangcl-toolset/15497>

## Building

With default toolset:

``` cmd
> cd e:/path/to/project
> mkdir build
> cd build
> cmake -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX="../install" -DUSING_CCACHE=YES ..
> cmake --build . --target install --config Release -- /m
```

With ClangCL toolset:

``` cmd
> cd e:/path/to/project
> mkdir build
> cd build
> cmake -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX="../install" -DUSING_CCACHE=YES -T ClangCL ..
> echo Fails
> cmake --build . --target install --config Release -- /m
> echo Succeeds
> cmake --build . --target install --config Release -- /m /p:CLToolExe=launch-cl.cmd
```
