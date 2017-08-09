/* Linker script to configure memory regions. 
 * Need modifying for a specific board. 
 *   FLASH.ORIGIN: starting address of flash
 *   FLASH.LENGTH: length of flash
 *   RAM.ORIGIN: starting address of RAM bank 0
 *   RAM.LENGTH: length of RAM bank 0
 */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x80000 /* 128K */
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x8000 /* 8K */
  RAM2 (rwx) : ORIGIN = 0x30000000, LENGTH = 0x2000 /* 8K */
}

/* Variables used by FreeRTOS-MPU. */
_Privileged_Functions_Region_Size = 32K;
_Privileged_Data_Region_Size = 512;

__FLASH_segment_start__ = ORIGIN( FLASH );
__FLASH_segment_end__ = __FLASH_segment_start__ + LENGTH( FLASH );

__privileged_functions_start__ = ORIGIN( FLASH );
__privileged_functions_end__ = __privileged_functions_start__ + _Privileged_Functions_Region_Size;

__SRAM_segment_start__ = ORIGIN( RAM );
__SRAM_segment_end__ = __SRAM_segment_start__ + LENGTH( RAM );

__privileged_data_start__ = ORIGIN( RAM );
__privileged_data_end__ = ORIGIN( RAM ) + _Privileged_Data_Region_Size;

INCLUDE arch/ldscripts/multi-ram.ld