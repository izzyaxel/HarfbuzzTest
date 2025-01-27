# HarfbuzzTest
 Testing HarfBuzz by itself, because it segfaults in another project

## Windows Setup
* Install [MSYS2](https://www.msys2.org/)
* Run `msys64/mingw64.exe`
* Install the neccessary packages in the mingw64 terminal using the following command:
* `pacman -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-harfbuzz mingw-w64-x86_64-freetype mingw-w64-x86_64-fontconfig`
* Clone this repository into `msys64/home/username/`
* Set up your toolchain to use the MSYS2 tools in CLion
* Build the project

## Other OS Setup
#### Untested, some includes may need to be added in the project files

* Install the necessary packages: `harfbuzz freetype fontconfig`
* Clone this repository
* Set up your toolchain
* Build the project
