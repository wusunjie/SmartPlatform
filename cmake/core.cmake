INCLUDE(${STM32_PLATFORM_TOP}/cmake/Boards/${STM32_PLATFORM_BOARD_NAME}.cmake)
SET(CMAKE_TOOLCHAIN_FILE ${STM32_PLATFORM_TOP}/cmake/toolchain-arm-none-eabi.cmake)

INCLUDE(${STM32_PLATFORM_TOP}/cmake/generic-gcc.cmake)
