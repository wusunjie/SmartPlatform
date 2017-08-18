# SmartPlatform

## prepare

1. sudo apt install cmake.
2. sudo apt install gcc-arm-none-eabi.

## build

mkdir out && cmake -DSTM32_PLATFORM_BOARD_NAME=stm3210c_eval helloworld -Bout && make -C out <br>

## currently supported board

1. stm3210c_eval
