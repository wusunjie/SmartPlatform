INCLUDE(${STM32_PLATFORM_TOP}/cmake/board-config-${STM32_PLATFORM_BOARD_NAME}.cmake)

IF(CMAKE_HOST_UNIX)
    SET(CMAKE_TOOLCHAIN_FILE ${STM32_PLATFORM_TOP}/cmake/toolchain-arm-none-eabi.cmake)
    INCLUDE(${STM32_PLATFORM_TOP}/cmake/generic-gcc.cmake)
ENDIF()
