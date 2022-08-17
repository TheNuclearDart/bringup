#ifndef __SDRAM_INIT_H
#define __SDRAM_INIT_H

#include <stdint.h>

enum class RamError
{
   FAILED,
   SUCCESS,
};

RamError sdram_init(void);

void     sdram_write_word(uint32_t word, uint32_t *dest);
uint32_t sdram_read_word(uint32_t *addr);
#endif