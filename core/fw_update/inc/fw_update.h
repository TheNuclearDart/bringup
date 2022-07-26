#ifndef __FW_UPDATE_H
#define __FW_UPDATE_H

#include <stdint.h>
#include "fw_image.h"
#include "memory_layout.h"

enum class fw_update_error_e
{
   GEN_FAILURE,
   INVALID_IMAGE,
   SUCCESS
};

ITCM_CODE fw_update_error_e fw_update(uint8_t *image_buffer_ptr);

#endif