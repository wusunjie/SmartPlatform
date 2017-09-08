INCLUDE_DIRECTORIES(${STM32_PLATFORM_TOP}/freertos/Source/include)

AUX_SOURCE_DIRECTORY(${STM32_PLATFORM_TOP}/freertos/Source SRC_LIST)
LIST(APPEND SRC_LIST ${STM32_PLATFORM_TOP}/freertos/Source/portable/Common/heap_3.c)

IF(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m4")
    INCLUDE_DIRECTORIES(${STM32_PLATFORM_TOP}/freertos/Source/portable/ARM_CM4F)
    AUX_SOURCE_DIRECTORY(${STM32_PLATFORM_TOP}/freertos/Source/portable/ARM_CM4F SRC_LIST)
    # LIST(APPEND SRC_LIST ${STM32_PLATFORM_TOP}/freertos/Source/portable/Common/mpu_wrappers.c)
ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m3")
    INCLUDE_DIRECTORIES(${STM32_PLATFORM_TOP}/freertos/Source/portable/ARM_CM3)
    AUX_SOURCE_DIRECTORY(${STM32_PLATFORM_TOP}/freertos/Source/portable/ARM_CM3 SRC_LIST)
    # LIST(APPEND SRC_LIST ${STM32_PLATFORM_TOP}/freertos/Source/portable/Common/mpu_wrappers.c)
ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m7")
    INCLUDE_DIRECTORIES(${STM32_PLATFORM_TOP}/freertos/Source/portable/ARM_CM7/r0p1)
    AUX_SOURCE_DIRECTORY(${STM32_PLATFORM_TOP}/freertos/Source/portable/ARM_CM7/r0p1 SRC_LIST)
ELSE()
    message(FATAL_ERROR
        "No C compiler for '${CMAKE_SYSTEM_PROCESSOR}'"
        " is supported yet."
    )
ENDIF()

