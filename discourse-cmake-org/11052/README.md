## .NET project with NuGet references

<https://discourse.cmake.org/t/netstandard-compilation-with-cmake/11052>

### Building and running

``` cmd
> mkdir build
> cd build
> cmake -G "Visual Studio 17 2022" ..
> cmake --build . --config Release

> Release\some.exe
Some: value
```
