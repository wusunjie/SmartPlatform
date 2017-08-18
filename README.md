# SmartPlatform

## prepare

1. gcc-arm-none-eabi toolchain is installed at /usr/local/bin.
2. toolchain root path is at /usr/local/lib/gcc/arm-none-eabi.

## build

mkdir out && cmake -DSTM32_PLATFORM_BOARD_NAME=stm3210c_eval helloworld -Bout && make -C out <br>

## currently supported board

1. stm3210c_eval
