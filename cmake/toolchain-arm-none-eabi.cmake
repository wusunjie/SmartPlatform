# this one is important
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR cortex-m4)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /usr/local/bin/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER /usr/local/bin/arm-none-eabi-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /usr/local/lib/gcc/arm-none-eabi)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/Generic-GCC.cmake)
