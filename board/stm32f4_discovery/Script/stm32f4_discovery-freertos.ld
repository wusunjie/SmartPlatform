/* Default memory layout. */
MEMORY
{
  ROM (rx)         : ORIGIN = 0x08000000, LENGTH = 1M
  RAM (rw)         : ORIGIN = 0x20000000, LENGTH = 128K
}

/*
 * Default stack sizes.
 * These are used by the startup in order to allocate stacks
 * for the different modes.
 */

__Main_Stack_Size = 2048 ;

/*
 * There will be a link error if there is not this amount of
 * RAM free at the end.
 */
_Minimum_Stack_Size = 1024 ;

INCLUDE arch/CMSIS/Device/Script/freertos.ld
