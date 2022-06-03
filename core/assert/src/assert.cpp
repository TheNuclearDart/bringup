#include "assert.h"
#include <stdint.h>
#include <stdio.h>

void assert_failed(uint8_t *file, uint32_t line, uint8_t *msg)
{
   printf("Assert! File: %s at Line: %ld Message: %s\r\n", file, line, msg);

   while(1);
}