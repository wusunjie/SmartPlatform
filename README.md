# SmartPlatform

## prepare

### Linux:

1. sudo apt install cmake.
2. sudo apt install gcc-arm-none-eabi.

### Windows:

1. download toolchain from [This link](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)

2. install it on your system.

3. download cmake from [This link](https://cmake.org/download)

4. install it on your system.

5. make sure there is at least a local build system on your platform, such as Visual Studio, Mingw, nmake.

## build

### Linux:

1. export ARM_NONE_EABI_HOME=/usr.

2. make BOARD_NAME=waveshare.

### Windows:

1. Set environment variable ARM_NONE_EABI_HOME=toolchain install path.

For example: C:/GNU_Tools_ARM_Embedded/6_2017_q2_update

2. Use CMake GUI, set the source directory to cmake, then choose your prefer build system.

3. Use your build system to open the project, then build in it.

## features

1. gprs network.

## currently supported board

1. waveshare
