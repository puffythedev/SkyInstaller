# SkyInstaller
The Global installer for Puffy's apps, intended for windows and linux (soon).

## Usage

Using this installer is simple:
- Open the file
- Click start
- Click install
- wait
- exit
And boom you have yourself a new app you can use
NOTE: All apps are stored in C:\PuffyApps\

## Building

To build this installer for your game/app
- Open CMakeLists.txt
- Replace lines 10 and 11 with your own data
```cmake
9   add_definitions(
10      -D APPNAME "SkyPong" # Application name
11      -D DOWNURL "https://github.com/puffythedev/SkyPong/releases/download/v0.1.5/SkyPong.zip" # Download URL
12  )
```
NOTE: downloaded file must be a ZIP archive
- Run the following commands:
```sh
.../SkyInstaller> mkdir build
.../SkyInstaller> cd build
.../SkyInstaller> cmake ..
.../SkyInstaller> cmake --build .
```
And boom! you got an installer in:
- SkyInstaller/build/Installer if using make/gcc
- SkyInstaller/build/Debug/Installer.exe if using msvc