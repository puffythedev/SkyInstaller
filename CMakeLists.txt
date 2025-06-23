cmake_minimum_required(VERSION 4.0)

project(SkyInstaller)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(outside/raylib)

add_definitions(
    -D APPNAME "SkyPong"
    -D DOWNURL "https://github.com/puffythedev/SkyPong/releases/download/v0.1.5/SkyPong.zip"
)
add_executable(Installer
    src/compat.cpp
    src/compat.hpp
    src/main.cpp
)
target_link_libraries(Installer PRIVATE raylib gdi32 winmm wininet shlwapi ole32)