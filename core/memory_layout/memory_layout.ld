/* License needed */

_min_stack_size = 0x400;
_min_heap_size  = 0x200; /*Not sure that this is needed */

MEMORY
{
   ITCM (xrw)    : ORIGIN = 0x00000000, LENGTH = 64K /*verify this length */
   DTCM (xrw)    : ORIGIN = 0x20000000, LENGTH = 64K
   SRAM (xrw)    : ORIGIN = 0x20010000, LENGTH = 240K
   BL_FLASH (xr) : ORIGIN = 0x08000000, LENGTH = 1024K /* This is the region defined for the bootloader in flash. This is the first image loaded.
   /* APP_FLASH(xr) : ORIGIN = ?, LENGTH = ?  This is the entire application image region. Both will live here. */
                                             /* Eventually, this will just be used to map out the region it gets stored, */
                                             /* but actually writing here will be handled by fwupdate

   /* Needs external RAM (if it exists on the board? 5-11-22 it does, but it needs to be added) */
}

/* __app_start__ = ORIGIN(APP_FLASH) */
/* __app_size__  = LEGNTH(APP_FLASH) */