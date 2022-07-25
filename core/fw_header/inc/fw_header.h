#ifndef __FW_HEADER_H
#define __FW_HEADER_H

#include <stdint.h>

// If this structure is updated, be sure to update header definition in build_image.py!
typedef struct _fw_image_header_t
{
   bool active;
   uint32_t fw_version; // if needed
   uint32_t image_size;
   uint32_t entry_addr;
   uint32_t crc32;
} fw_image_header_t;

typedef struct _segment_header_t
{
   uint32_t dest_addr;
   uint32_t size;
   uint8_t  rsvd[8];
} segment_header_t;

#endif