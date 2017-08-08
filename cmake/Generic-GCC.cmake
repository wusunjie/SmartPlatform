SET(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")
SET(CMAKE_C_FLAGS "-fno-common -ffunction-sections -fdata-sections")

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m4")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m3")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -march=armv7-m -mthumb -msoft-float")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a9")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a9 -march=armv7-a -mthumb -mfloat-abi=hard -mfpu=neon")
else()
    message(FATAL_ERROR
        "No C compiler for '${CMAKE_SYSTEM_PROCESSOR}'"
        " is supported yet."
    )
endif()

