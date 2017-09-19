# this one is important
SET(CMAKE_SYSTEM_NAME Generic)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(ARM_NONE_EABI_PATH C:/GNU_Tools_ARM_Embedded/6_2017_q2_update)
SET(CMAKE_C_COMPILER   ${ARM_NONE_EABI_PATH}/bin/arm-none-eabi-gcc.exe)
SET(CMAKE_CXX_COMPILER ${ARM_NONE_EABI_PATH}/bin/arm-none-eabi-g++.exe)
SET(OBJ_COPY ${ARM_NONE_EABI_PATH}/bin/arm-none-eabi-objcopy)
SET(OBJ_SIZE ${ARM_NONE_EABI_PATH}/bin/arm-none-eabi-size)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH ${ARM_NONE_EABI_PATH}/lib/gcc/arm-none-eabi)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
