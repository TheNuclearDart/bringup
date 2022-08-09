#ifndef __FW_IMAGE_H
#define __FW_IMAGE_H

#include <stdint.h>

enum class image_error_e
{
   GENERIC_FAILURE,
   INVALID_IMAGE,
   NO_VALID_IMAGES,
   SUCCESS
};

void          fw_image_init(uint32_t (*crc_func_ptr)(uint32_t *buffer, uint32_t length), int (*print_func_ptr)(const char *format, ...));
image_error_e fw_image_load(void);
void          fw_image_execute(void);
image_error_e fw_image_validate(uint8_t *image_buffer_ptr);

#endif