IF(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m4")
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m3")
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -march=armv7-m -mthumb -msoft-float")
ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m7")
    SET(ARCH_FLAGS "${CMAKE_C_FLAGS} -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16")
ELSE()
    message(FATAL_ERROR
        "No C compiler for '${CMAKE_SYSTEM_PROCESSOR}'"
        " is supported yet."
    )
ENDIF()

SET(LINK_SCRIPT ${STM32_PLATFORM_TOP}/board/${STM32_PLATFORM_BOARD_NAME}/Script/${STM32_PLATFORM_BOARD_NAME}-freertos.ld)

SET(CMAKE_EXE_LINKER_FLAGS "-L${STM32_PLATFORM_TOP} -Wl,--gc-sections --specs=nosys.specs -T${LINK_SCRIPT}" CACHE INTERNAL "" FORCE)
SET(CMAKE_C_FLAGS "${ARCH_FLAGS} ${STARTUP_DEFS} -Os -ffunction-sections -fdata-sections" CACHE INTERNAL "" FORCE)

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
