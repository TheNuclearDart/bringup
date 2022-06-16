#ifndef __FW_HEADER_H
#define __FW_HEADER_H

#include <stdint.h>

// If this structure is updated, be sure to update header in write_crc.py
typedef struct _fw_image_header_t
{
   bool active;
   uint32_t fw_version; // if needed
   uint32_t image_size;
   uint32_t crc32;
   uint8_t  rsvd[112]; // Pad out to fill space
} fw_image_header_t;

fw_image_header_t *fw_header_get_current_header(void);
uint32_t           fw_header_get_header_size(void);

#endif