# SmartPlatform

## prepare

1. gcc-arm-none-eabi toolchain is installed at /usr/local/bin.
2. toolchain root path is at /usr/local/lib/gcc/arm-none-eabi.

## build

1. enter example folder ~/SmartPlatform/helloworld: <br>
cd ~/SmartPlatform/helloworld
2. create out folder and enter it for out-of-source build: <br>
mkdir out && cd out
3. start building: <br>
cmake -DSTM32_PLATFORM_BOARD_NAME=stm3210c_eval .. <br>
make

## currently supported board

1. stm3210c_eval
