# Nerves of Steel Remake
Remake of Nerves of Steel

A remake of the 1995 DOS game [Nerves of Steel](https://en.wikipedia.org/wiki/Nerves_of_Steel_(video_game)) by Rainmaker Software and BnB Software.

**This is still an early work in progress!**

## Build Instructions

### MinGW

- Download and run the installer from https://www.msys2.org/
- Install the build dependencies from the shell
``` shell
pacman -S git mingw-w64-x86_64-toolchain make unzip
```
- Build the executable
``` shell
make
```

### MinGW (Linux cross compile)
- Ubuntu / Debian
- Install the build toolchain
``` shell
sudo apt install gcc-mingw-w64-x86-64  g++-mingw-w64-x86-64 make
```

- Build the executable
``` shell
CONFIG_W64=1 make
```

### Linux

- Ubuntu / Debian

- Install the build toolchain
``` shell
sudo apt install build-essential libx11-dev libxext-dev libxft-dev libxinerama-dev libfontconfig-dev libwayland-dev libcairo-dev libxfixes-dev libxcursor-dev libharfbuzz-dev libpango1.0-dev libxkbcommon-dev libdecor-0-dev libdbus-1-dev
```
- Build the executable
``` shell
make
```

## Installing game content

The original game files are required to run the game. A copy of the original DOS game is hosted on the Internet Archive.

```
wget "https://archive.org/download/exov5_2/eXo/eXoDOS/Nerves%20of%20Steel%20%281995%29.zip"
unzip "Nerves of Steel (1995).zip"
```
