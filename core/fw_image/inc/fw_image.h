#ifndef __FW_IMAGE_H
#define __FW_IMAGE_H

#include <stdint.h>

typedef struct fw_image_header_t
{
   bool active;
   uint32_t fw_version; // if needed
   uint32_t image_size;
   uint32_t crc32;
} fw_image_header;

enum class image_error_e
{
   SUCCESS,
   GENERIC_FAILURE,
   INVALID_IMAGE,
};

image_error_e fw_image_load(void);
void          fw_image_execute(void);

#endif