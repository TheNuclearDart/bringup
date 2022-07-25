#ifndef __SHARED_DATA_H
#define __SHARED_DATA_H

#include <stdint.h>
#include "fw_header.h"

typedef struct _shared_data_t
{
   fw_image_header_t active_fw_header;
   uint8_t     rsvd[512 - sizeof(fw_image_header_t)];
} shared_data_t;

fw_image_header_t *shared_data_get_active_fw_header(void);
void               shared_data_populate_active_fw_header(fw_image_header_t *source_ptr);
#endif