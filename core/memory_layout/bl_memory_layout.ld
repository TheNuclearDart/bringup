INCLUDE memory_layout.ld

ENTRY(reset_handler)

SECTIONS
{
   .shared_data (NOLOAD):
   {
      . = ALIGN(4);
      __shared_data_start__ = .;
      KEEP(*(.shared_data))
      . = ALIGN(4);
      __shared_data_end__ = .;
   } > SRAM

   .isr_vector :
   {
      . = ALIGN(4);
      KEEP(*(.isr_vector))
      . = ALIGN(4);
   } > BL_FLASH

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
   } > BL_FLASH

   .rodata :
   {
      . = ALIGN(4);
      *(.rodata) /* All constant data goes in flash */
      *(.roadata*)
      . = ALIGN(4);
   } > BL_FLASH

   .ARM.extab : 
   { 
      *(.ARM.extab* .gnu.linkonce.armextab.*) 
   } > BL_FLASH
  
  .ARM : 
  {
      __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
  } > BL_FLASH

  /* These are things I need to understand */
  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > BL_FLASH
  
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > BL_FLASH
  
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > BL_FLASH

   _sidata = LOADADDR(.data); /* Expose the load address (RAM address) for .data */
   .data :
   {
      . = ALIGN(4);
      _sdata = .;
      *(.data)
      *(.data*)
      . = ALIGN(4);
      _edata = .;
   } > DTCM AT > BL_FLASH

   .bss (NOLOAD) :
   {
      . = ALIGN(4);
      _sbss = .;
      *(.bss)
      *(.bss*)
      *(COMMON)
      . = ALIGN(4);
      _ebss = .;
   } > DTCM

   /* The stack must be the last RAM rule (I think) */
   .stack (NOLOAD) :
   {
      . = ALIGN(8);
      end = .;
      . = . + _min_heap_size;
      . = . + _min_stack_size;
      . = ALIGN(8);
      _estack = .;
   } > DTCM

   /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}