# this one is important
SET(CMAKE_SYSTEM_NAME Generic)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    SET(ARM_NONE_EABI_PATH C:/GNU_Tools_ARM_Embedded/6_2017_q2_update)
    SET(TOOLCHAIN_SUFFIX .exe)
ELSE()
    SET(ARM_NONE_EABI_PATH /usr)
ENDIF()

SET(CMAKE_C_COMPILER   ${ARM_NONE_EABI_PATH}/bin/arm-none-eabi-gcc${TOOLCHAIN_SUFFIX})
SET(CMAKE_CXX_COMPILER ${ARM_NONE_EABI_PATH}/bin/arm-none-eabi-g++${TOOLCHAIN_SUFFIX})
SET(OBJ_COPY ${ARM_NONE_EABI_PATH}/bin/arm-none-eabi-objcopy${TOOLCHAIN_SUFFIX})
SET(OBJ_SIZE ${ARM_NONE_EABI_PATH}/bin/arm-none-eabi-size${TOOLCHAIN_SUFFIX})

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH ${ARM_NONE_EABI_PATH}/lib/gcc/arm-none-eabi)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
