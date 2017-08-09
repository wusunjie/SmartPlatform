if(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m4")
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
    LIST(APPEND SRC_LIST ${STM32_PLATFORM_TOP}/arch/startup/startup_ARMCM4.S)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m3")
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -march=armv7-m -mthumb -msoft-float")
    LIST(APPEND SRC_LIST ${STM32_PLATFORM_TOP}/arch/startup/startup_ARMCM3.S)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a9")
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a9 -march=armv7-a -mthumb -mfloat-abi=hard -mfpu=neon")
else()
    message(FATAL_ERROR
        "No C compiler for '${CMAKE_SYSTEM_PROCESSOR}'"
        " is supported yet."
    )
endif()

SET(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs -T${LINK_SCRIPT}" CACHE INTERNAL "")
SET(CMAKE_C_FLAGS "${ARCH_FLAGS} ${STARTUP_DEFS} -Os -flto -ffunction-sections -fdata-sections")
