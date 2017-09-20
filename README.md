# SmartPlatform

## prepare

### Linux:

1. sudo apt install cmake.
2. sudo apt install gcc-arm-none-eabi.

### Windows:

1. download [gcc-arm-none-eabi](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) and install it.

2. download [cmake](https://cmake.org/download) and install it.

3. make sure there is at least a local build system on your platform, such as Visual Studio, Mingw, nmake.

## build

### Linux:

1. make BOARD_NAME=waveshare.

### Windows:

1. Use CMake GUI, set the source directory to cmake, then choose your prefer build system.

2. add macro define in CMake GUI for STM32_PLATFORM_BOARD_NAME to your board name, such as waveshare.

3. Use your build system to open the project, then build in it.

## features

1. gprs network.

## currently supported board

1. waveshare
