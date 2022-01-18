/* License needed */

.syntax unified
.cpu cortex-m7
.fpu softvfp
.thumb

/* The following symbols must be provided in the linker script */
/* The start address for initialized .data values. This is (typically) the FLASH address
   for the .data section */
.word _sidata
/* Start and end addresses for the .data section. This is (typically) the RAM address */
.word _sdata
.word _edata
/* Start and end of .bss section. This needs to be zeroed in the startup  */
.word _sbss
.word _ebss

/* Are these globals needed? */
.global vector_table
.global default_handler


/* @brief First function executed when the processor boots. Needs to:
          1.) Set the stack pointer 
          2.) Zero the .bss section
          3.) Copy .data to destination*/
.section  .text.reset_handler
.weak  reset_handler
.type  reset_handler, %function
reset_handler:
   ldr sp, =_estack /* Set the stack pointer to the end of the stack (grows up) */

   /* Zero the bss section by looping */

   ldr r2, =_sbss /* r2 holds the address to write/store to */
   ldr r4, =_ebss /* r4 holds the address to end on */
   movs r3, #0 /* r3 contains const 0? */
   bl LoopZeroFill
   ldr r0, =_sdata 
   ldr r1, =_edata
   ldr r2, =_sidata
   movs r3, #0 /* Incrementing offset */
   bl LoopCopyDataSection
   //bl SystemInit /* System init function (if needed, I think this is CMSIS) */
   //bl __libc_init_array /* Init libc (dunno how this works yet) */

   //bl main /* Jump to application! */
   b Infinite_Loop
   /* Need to do something here if main returns eventually */
.size reset_handler, .-reset_handler

/* This should probably be like an assert or something at some point
   Or some type C-function that asserts and prints some info */
   .section .text.default_handler,"ax",%progbits //dunno what any of this means?
default_handler:
   b Infinite_Loop
   .size default_handler, .-default_handler

ZeroFill: /* Will intentionally fall through to LoopZeroFill */
   str r3, [r2] /* stores the value of r3 (#0) at address in r2 */
   adds r2, r2, #4 /* increment address by one word */

LoopZeroFill:
   cmp r2, r4 /*check if we have reache _ebss */
   bcc ZeroFill /* If r2 != r4, branch to FillZero */
   bx lr /* Return if we are done */

CopyDataSection:
   ldr r4, [r2, r3] /* load value at addr(r2+r3) into r4 */
   str r4, [r0, r3] /* now store that value at it's destination */
   adds r3, r3, #4 /* increment offset by one word */
   /*intentional fall through */

LoopCopyDataSection:
   adds r4, r0, r3
   cmp r4, r1
   bcc CopyDataSection
   bx lr /* return if we are done */


Infinite_Loop:
   b Infinite_Loop


