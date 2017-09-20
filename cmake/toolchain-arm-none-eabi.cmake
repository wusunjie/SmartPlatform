# this one is important
SET(CMAKE_SYSTEM_NAME Generic)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)


IF(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    SET(TOOLCHAIN_SUFFIX .exe)
ENDIF()

# specify the cross compiler
SET(CMAKE_C_COMPILER   arm-none-eabi-gcc${TOOLCHAIN_SUFFIX})
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++${TOOLCHAIN_SUFFIX})
SET(OBJ_COPY arm-none-eabi-objcopy${TOOLCHAIN_SUFFIX})
SET(OBJ_SIZE arm-none-eabi-size${TOOLCHAIN_SUFFIX})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
