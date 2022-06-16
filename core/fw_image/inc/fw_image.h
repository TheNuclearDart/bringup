#ifndef __FW_IMAGE_H
#define __FW_IMAGE_H

#include <stdint.h>

enum class image_error_e
{
   SUCCESS,
   GENERIC_FAILURE,
   INVALID_IMAGE,
};

image_error_e fw_image_load(void);
void          fw_image_execute(void);

#endif