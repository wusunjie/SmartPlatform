# this one is important
SET(CMAKE_SYSTEM_NAME Generic)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /usr/bin/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++)
SET(OBJ_COPY /usr/bin/arm-none-eabi-objcopy)
SET(OBJ_SIZE /usr/bin/arm-none-eabi-size)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /usr/lib/gcc/arm-none-eabi)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
