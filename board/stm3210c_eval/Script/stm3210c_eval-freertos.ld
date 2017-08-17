/* Default memory layout. */
MEMORY
{
  ROM (rx)         : ORIGIN = 0x00, LENGTH = 0x80000
  RAM (rw)         : ORIGIN = 0x20000000, LENGTH = 0x8000
}

/* Variables used by FreeRTOS-MPU. */
_Privileged_Functions_Region_Size = 32K;
_Privileged_Data_Region_Size = 512;

__FLASH_segment_start__ = ORIGIN( ROM );
__FLASH_segment_end__ = __FLASH_segment_start__ + LENGTH( ROM );

__privileged_functions_start__ = ORIGIN( ROM );
__privileged_functions_end__ = __privileged_functions_start__ + _Privileged_Functions_Region_Size;

__SRAM_segment_start__ = ORIGIN( RAM );
__SRAM_segment_end__ = __SRAM_segment_start__ + LENGTH( RAM );

__privileged_data_start__ = ORIGIN( RAM );
__privileged_data_end__ = ORIGIN( RAM ) + _Privileged_Data_Region_Size;

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
