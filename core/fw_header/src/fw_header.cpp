#include "fw_header.h"

#if !BOOTLOADER_BUILD
__attribute__((section(".fw_header"))) fw_image_header_t fw_image_header; // To be filled out elsewhere?
#endif

fw_image_header_t fw_header_get_current_header(void)
{
   return fw_image_header;
}

uint32_t fw_header_get_header_size(void)
{
   return sizeof(fw_image_header_t);
}