# this one is important
SET(CMAKE_SYSTEM_NAME Generic)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /usr/local/bin/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER /usr/local/bin/arm-none-eabi-g++)
SET(CMAKE_LINKER /usr/local/bin/arm-none-eabi-ld)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /usr/local/lib/gcc/arm-none-eabi)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
