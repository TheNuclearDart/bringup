#ifndef __FW_IMAGE_H
#define __FW_IMAGE_H

#include <stdint.h>

enum class image_error_e
{
   SUCCESS,
   GENERIC_FAILURE,
   INVALID_IMAGE,
};

void          fw_image_init(uint32_t (*func_ptr)(uint32_t *buffer, uint32_t length));
image_error_e fw_image_load(void);
void          fw_image_execute(void);

#endif