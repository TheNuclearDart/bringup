#include "fw_header.h"

#if !BOOTLOADER_BUILD
__attribute__((section(".fw_header"))) constexpr fw_image_header_t fw_image_header = 
{
   .active     = true,
   .fw_version = 0xFFFFFFFF,
   .image_size = 0xFFFFFFFF,
   .crc32      = 0xFFFFFFFF,
};
#endif

fw_image_header_t fw_header_get_current_header(void)
{
   return fw_image_header;
}

uint32_t fw_header_get_header_size(void)
{
   return sizeof(fw_image_header_t);
}