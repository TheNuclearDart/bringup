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

   .fw_header :
   {
      . = ALIGN(4);
      KEEP(*(.fw_header))
      . = ALIGN(4);
   } > SRAM

   .isr_vector :
   {
      . = ALIGN(128);
      KEEP(*(.isr_vector))
      . = ALIGN(128);
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
   } > SRAM

   .rodata :
   {
      . = ALIGN(4);
      *(.rodata) /* All constant data goes in flash */
      *(.roadata*)
      . = ALIGN(4);
   } > SRAM

   .ARM.extab : 
   { 
      *(.ARM.extab* .gnu.linkonce.armextab.*) 
   } > SRAM
  
  .ARM : 
  {
      __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
  } > SRAM

  /* These are things I need to understand */
  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > SRAM
  
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > SRAM
  
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > SRAM

   _sidata = LOADADDR(.data); /* Expose the load address (RAM address) for .data */
   .data :
   {
      . = ALIGN(4);
      _sdata = .;
      *(.data)
      *(.data*)
      . = ALIGN(4);
      _edata = .;
   } > SRAM

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
      end = .;
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