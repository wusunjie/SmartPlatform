IF(SMART_PLATFORM_MCU_SERIES STREQUAL "STM32F4xx")
    SET(CMAKE_SYSTEM_PROCESSOR cortex-m4)
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
ELSEIF(SMART_PLATFORM_MCU_SERIES STREQUAL "STM32F3xx")
    SET(CMAKE_SYSTEM_PROCESSOR cortex-m4)
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
ELSEIF(SMART_PLATFORM_MCU_SERIES STREQUAL "STM32F2xx")
    SET(CMAKE_SYSTEM_PROCESSOR cortex-m3)
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -march=armv7-m -mthumb -msoft-float")
ELSEIF(SMART_PLATFORM_MCU_SERIES STREQUAL "STM32F1xx")
    SET(CMAKE_SYSTEM_PROCESSOR cortex-m3)
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -march=armv7-m -mthumb -msoft-float")
ELSE()
    message(FATAL_ERROR
        "No C compiler for '${CMAKE_SYSTEM_PROCESSOR}'"
        " is supported yet."
    )
ENDIF()

INCLUDE(${SMART_PLATFORM_TOP}/cmake/generic-gcc-stm32.cmake)
INCLUDE_DIRECTORIES(${SMART_PLATFORM_TOP}/board/common/Include)
AUX_SOURCE_DIRECTORY(${SMART_PLATFORM_TOP}/board/common/Source SRC_LIST)

INCLUDE(${SMART_PLATFORM_TOP}/cmake/rtos-freertos.cmake)
SET(LINK_SCRIPT ${SMART_PLATFORM_TOP}/board/${SMART_PLATFORM_BOARD_NAME}/Script/freertos.ld)

# INCLUDE(${SMART_PLATFORM_TOP}/cmake/lwip.cmake)

SET(CMAKE_EXE_LINKER_FLAGS "-L${SMART_PLATFORM_TOP} -Wl,--gc-sections -Wl,--no-wchar-size-warning -Wl,-Map,${SMART_PLATFORM_BOARD_NAME}.map --specs=nano.specs --specs=nosys.specs -T${LINK_SCRIPT}" CACHE INTERNAL "" FORCE)
SET(CMAKE_C_FLAGS_RELEASE "${ARCH_FLAGS} -Os -flto -ffunction-sections -fdata-sections" CACHE INTERNAL "" FORCE)
SET(CMAKE_C_FLAGS_DEBUG "${ARCH_FLAGS} -g -ffunction-sections -fdata-sections" CACHE INTERNAL "" FORCE)

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
