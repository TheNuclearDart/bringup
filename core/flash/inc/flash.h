#ifndef __FLASH_H
#define __FLASH_H

#include <stdint.h>

enum class flash_error_e
{
   GEN_FAILURE,
   SUCCESS
};

flash_error_e flash_write(uint8_t *dest_ptr, uint8_t *src_ptr, uint32_t size);
flash_error_e flash_clear_byte(uint8_t *byte_ptr);

#endif