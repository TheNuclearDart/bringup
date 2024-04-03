/* License needed */

_min_bl_stack_size  = 0x400;
_min_bl_heap_size   = 0x200;
_min_app_stack_size = 0x400;
_min_app_heap_size  = 0x200; /*Not sure that this is needed */

MEMORY
{
   ITCM      (xrw) : ORIGIN = 0x00000000, LENGTH = 16K /*verify this length (I think it is wrong) */
   DTCM      (xrw) : ORIGIN = 0x20000000, LENGTH = 64K
   SRAM      (xrw) : ORIGIN = 0x20010000, LENGTH = 240K /* Should this be bigger? */
   BL_FLASH  (xr)  : ORIGIN = 0x08000000, LENGTH = 64K  /* This is the region defined for the bootloader in flash. This is the first image loaded. */
   APP_FLASH (xr)  : ORIGIN = 0x08010000, LENGTH = 0x70000 /* Make this 0x70000 when we can support it(448KB... weird, probably bad size). Rignt now the entire image is in SRAM, so it needs to match that size! */
   SDRAM     (xrw) : ORIGIN = 0xC0000000, LENGTH = 8M  /* I believe this is correct, that the SDRAM is only 64Mbit (8Mbyte!!), and connected to FMC SDRAM bank 1 */
}

__sdram_start__ = ORIGIN(SDRAM);
__sdram_size__  = LENGTH(SDRAM);
__sram_start__  = ORIGIN(SRAM);
__app_start__   = ORIGIN(APP_FLASH);
__app_size__    = LENGTH(APP_FLASH);