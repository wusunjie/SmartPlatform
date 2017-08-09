IF(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m4")
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
    SET_PROPERTY(SOURCE ${STM32_PLATFORM_TOP}/arch/startup/startup_ARMCM4.S PROPERTY LANGUAGE C)
    LIST(APPEND SRC_LIST ${STM32_PLATFORM_TOP}/arch/startup/startup_ARMCM4.S)
ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m3")
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -march=armv7-m -mthumb -msoft-float")
    SET_PROPERTY(SOURCE ${STM32_PLATFORM_TOP}/arch/startup/startup_ARMCM3.S PROPERTY LANGUAGE C)
    LIST(APPEND SRC_LIST ${STM32_PLATFORM_TOP}/arch/startup/startup_ARMCM3.S)
ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a9")
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a9 -march=armv7-a -mthumb -mfloat-abi=hard -mfpu=neon")
ELSE()
    message(FATAL_ERROR
        "No C compiler for '${CMAKE_SYSTEM_PROCESSOR}'"
        " is supported yet."
    )
ENDIF()

SET(LINK_SCRIPT ${STM32_PLATFORM_TOP}/board/${STM32_PLATFORM_BOARD_NAME}/${STM32_PLATFORM_BOARD_NAME}.ld)
SET(CMAKE_EXE_LINKER_FLAGS "-L${STM32_PLATFORM_TOP} -Wl,--gc-sections --specs=nosys.specs -T${LINK_SCRIPT}" CACHE INTERNAL "")
SET(CMAKE_C_FLAGS "${ARCH_FLAGS} ${STARTUP_DEFS} -Os -flto -ffunction-sections -fdata-sections")
