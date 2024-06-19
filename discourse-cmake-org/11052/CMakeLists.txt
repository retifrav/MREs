cmake_minimum_required(VERSION 3.22)

project(
    "some"
    LANGUAGES CSharp
)

add_executable(${CMAKE_PROJECT_NAME})

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/some.cs
)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${sources}
)

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
        DOTNET_SDK "Microsoft.NET.Sdk"
        DOTNET_TARGET_FRAMEWORK "net8.0"
        VS_PACKAGE_REFERENCES "Newtonsoft.Json_13.0.1"
)
