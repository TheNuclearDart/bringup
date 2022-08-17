INCLUDE memory_layout.ld

ENTRY(reset_handler)

SECTIONS
{
   /* Will this work? Block off the stack for the BL in SRAM, so that it isn't overwritten when loading? But then how do we reclaim it?
      This might work, but we need a way to either have in the segment header the correct start address, or step over this chunk in the
      bootloader, as right now we are just hitting the start of SRAM.
   .bl_stack (NOLOAD) :
   {
      . = ALIGN(8);
      end = .;
      . = . + _min_heap_size;
      . = . + _min_stack_size;
      . = ALIGN(8);
      _eblstack = .;
   } > SRAM */

   /* ITCM Sections */
   .itcm_code :
   {
      . = ALIGN(4);
      *(.itcm_code)
      *(.itcm_code*)
      *(.itcm_data)
      *(.itcm_data*)
      . = ALIGN(4);
   } > ITCM

   /* DTCM Sections */
   .dtcm_code :
   {
      . = ALIGN(4);
      *(.dtcm_code)
      *(.dtcm_code*)
      *(.dtcm_data)
      *(.dtcm_data*)
      . = ALIGN(4);
   } > DTCM

   /* SRAM Sections */
   .shared_data (NOLOAD):
   {
      . = ALIGN(4);
      __shared_data_start__ = .;
      *(.shared_data)
      . = ALIGN(4);
      __shared_data_end__ = .;
   } > SRAM

   .isr_vector :
   {
      . = ALIGN(128);
      KEEP(*(.isr_vector))
      . = ALIGN(128);
   } > SDRAM

   .sram_code :
   {
      . = ALIGN(4);
      *(.sram_code)
      *(.sram_code*)
      *(.sram_data)
      *(.sram_data*)
      . = ALIGN(4);
   } > SRAM

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
   } > SDRAM

   .rodata :
   {
      . = ALIGN(4);
      *(.rodata) /* All constant data goes in flash */
      *(.roadata*)
      . = ALIGN(4);
   } > SDRAM

   .ARM.extab : 
   { 
      *(.ARM.extab* .gnu.linkonce.armextab.*) 
   } > SDRAM
  
  .ARM : 
  {
      __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
  } > SDRAM

  /* These are things I need to understand */
  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > SDRAM
  
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > SDRAM
  
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > SDRAM

   _sidata = LOADADDR(.data); /* Expose the load address (RAM address) for .data */
   .data :
   {
      . = ALIGN(4);
      _sdata = .;
      *(.data)
      *(.data*)
      . = ALIGN(4);
      _edata = .;
   } > SDRAM

   .bss (NOLOAD) :
   {
      . = ALIGN(4);
      _sbss = .;
      *(.bss)
      *(.bss*)
      *(COMMON)
      . = ALIGN(4);
      _ebss = .;
   } > SDRAM

   /* The stack must be the last RAM rule (I think) */
   .stack (NOLOAD) :
   {
      . = ALIGN(8);
      end = .;
      . = . + _min_heap_size;
      . = . + _min_stack_size;
      . = ALIGN(8);
      _estack = .;
   } > SDRAM

   /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}