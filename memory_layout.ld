/* License needed */

ENTRY(reset_handler)

_min_stack_size = 0x400;
_min_heap_size  = 0x200; /*Not sure that this is needed */

MEMORY
{
   ITCM (xrw) : ORIGIN = 0x00000000, LENGTH = 64K /*verify this length */
   DTCM (xrw) : ORIGIN = 0x20000000, LENGTH = 64K
   SRAM (xrw) : ORIGIN = 0x20010000, LENGTH = 240K
   FLASH (xr) : ORIGIN = 0x08000000, LENGTH = 1024K
   /* Needs external RAM (if it exists on the board? 5-11-22 it does, but it needs to be added) */
}

SECTIONS
{
   .isr_vector :
   {
      . = ALIGN(4);
      KEEP(*(.isr_vector))
      . = ALIGN(4);
   } > FLASH

   .text :
   {
      . = ALIGN(4);
      *(.text) /* all .text sections */
      *(.text*)
      *(.glue_7)         /* glue arm to thumb code */
      *(.glue_7t)        /* glue thumb to arm code */
      *(.eh_frame)

      KEEP(*(.init))
      KEEP(*(.fini))

      . = ALIGN(4);
      _etext = .;
   } > FLASH

   .rodata :
   {
      . = ALIGN(4);
      *(.rodata) /* All constant data goes in flash */
      *(.roadata*)
      . = ALIGN(4);
   } > FLASH

   .ARM.extab : 
   { 
      *(.ARM.extab* .gnu.linkonce.armextab.*) 
   } > FLASH
  
  .ARM : 
  {
      __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
  } > FLASH

  /* These are things I need to understand */
  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > FLASH
  
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > FLASH
  
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > FLASH

   _sidata = LOADADDR(.data); /* Expose the load address (RAM address) for .data */
   .data :
   {
      . = ALIGN(4);
      _sdata = .;
      *(.data)
      *(.data*)
      . = ALIGN(4);
      _edata = .;
   } > SRAM AT > FLASH

   .bss (NOLOAD) :
   {
      . = ALIGN(4);
      _sbss = .;
      *(.bss)
      *(.bss*)
      *(COMMON)
      . = ALIGN(4);
      _ebss = .;
   } > SRAM

   /* The stack must be the last RAM rule (I think) */
   .stack (NOLOAD) :
   {
      . = ALIGN(8);
      . = . + _min_heap_size;
      . = . + _min_stack_size;
      . = ALIGN(8);
      _estack = .;
   } > SRAM

   /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}