#include "shared_data.h"
#include "fw_header.h"

#include <string.h>

__attribute__((section(".shared_data"))) shared_data_t shared_data; // To be filled out elsewhere?

fw_image_header_t *shared_data_get_active_fw_header(void)
{
   return &shared_data.active_fw_header;
}

void shared_data_populate_active_fw_header(fw_image_header_t *source_ptr)
{
   memcpy(&shared_data.active_fw_header, source_ptr, sizeof(fw_image_header_t));
}