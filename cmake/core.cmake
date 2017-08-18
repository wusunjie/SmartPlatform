INCLUDE(${STM32_PLATFORM_TOP}/cmake/board-config-${STM32_PLATFORM_BOARD_NAME}.cmake)
IF(CMAKE_HOST_UNIX)
    SET(CMAKE_TOOLCHAIN_FILE ${STM32_PLATFORM_TOP}/cmake/toolchain-arm-none-eabi.cmake)
ENDIF()

IF(STM32_PLATFORM_BUILD_APP STREQUAL "true")
    IF(CMAKE_HOST_UNIX)
        INCLUDE(${STM32_PLATFORM_TOP}/cmake/generic-gcc-app.cmake)
    ENDIF()
ELSE()
    IF(CMAKE_HOST_UNIX)
        INCLUDE(${STM32_PLATFORM_TOP}/cmake/generic-gcc-platform.cmake)
    ENDIF()
ENDIF()


