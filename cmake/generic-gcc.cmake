IF(STM32_PLATFORM_MCU_SERIES STREQUAL "STM32F4xx")
    SET(CMAKE_SYSTEM_PROCESSOR cortex-m4)
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
    IF(STM32_PLATFORM_BUILD_APP STREQUAL "false")
        INCLUDE(${STM32_PLATFORM_TOP}/cmake/generic-gcc-stm32.cmake)
    ENDIF()
ELSEIF(STM32_PLATFORM_MCU_SERIES STREQUAL "STM32F3xx")
    SET(CMAKE_SYSTEM_PROCESSOR cortex-m4)
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
    IF(STM32_PLATFORM_BUILD_APP STREQUAL "false")
        INCLUDE(${STM32_PLATFORM_TOP}/cmake/generic-gcc-stm32.cmake)
    ENDIF()
ELSEIF(STM32_PLATFORM_MCU_SERIES STREQUAL "STM32F2xx")
    SET(CMAKE_SYSTEM_PROCESSOR cortex-m3)
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -march=armv7-m -mthumb -msoft-float")
    IF(STM32_PLATFORM_BUILD_APP STREQUAL "false")
        INCLUDE(${STM32_PLATFORM_TOP}/cmake/generic-gcc-stm32.cmake)
    ENDIF()
ELSEIF(STM32_PLATFORM_MCU_SERIES STREQUAL "STM32F1xx")
    SET(CMAKE_SYSTEM_PROCESSOR cortex-m3)
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -march=armv7-m -mthumb -msoft-float")
    IF(STM32_PLATFORM_BUILD_APP STREQUAL "false")
        INCLUDE(${STM32_PLATFORM_TOP}/cmake/generic-gcc-stm32.cmake)
    ENDIF()
ELSE()
    message(FATAL_ERROR
        "No C compiler for '${CMAKE_SYSTEM_PROCESSOR}'"
        " is supported yet."
    )
ENDIF()

IF(STM32_PLATFORM_BUILD_APP STREQUAL "false")
    INCLUDE_DIRECTORIES(${STM32_PLATFORM_TOP}/board/common/Include)
    LIST(APPEND SRC_LIST ${STM32_PLATFORM_TOP}/board/common/Source/retarget.c)
ENDIF()

INCLUDE(${STM32_PLATFORM_TOP}/cmake/rtos-freertos.cmake)
SET(LINK_SCRIPT ${STM32_PLATFORM_TOP}/board/${STM32_PLATFORM_BOARD_NAME}/Script/freertos.ld)

SET(CMAKE_EXE_LINKER_FLAGS "-L${STM32_PLATFORM_TOP} -Wl,--gc-sections -Wl,-Map,${STM32_PLATFORM_BOARD_NAME}.map --specs=nosys.specs -T${LINK_SCRIPT}" CACHE INTERNAL "" FORCE)
SET(CMAKE_C_FLAGS "${ARCH_FLAGS} ${STARTUP_DEFS} -g -Os -flto -ffunction-sections -fdata-sections" CACHE INTERNAL "" FORCE)

# post-process elf files into .hex files:
FUNCTION(CREATE_IMAGE target_name)
    ADD_CUSTOM_COMMAND(TARGET ${target_name}
        POST_BUILD
        COMMAND "${OBJ_COPY}" -O ihex ${target_name} ${target_name}.hex
        COMMENT "Creating ${target_name}.hex ..."
        COMMAND "${OBJ_SIZE}" ${target_name}
        VERBATIM
    )
ENDFUNCTION()
