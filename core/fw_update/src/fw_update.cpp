// Need license

#include "fw_update.h"

#include "flash.h"
#include "fw_header.h"
#include "fw_image.h"
#include "memory_layout.h"
#include "stm32f7xx.h"

fw_update_error_e fw_update(uint8_t *image_buffer_ptr)
{
   // 1. Validate the image in the buffer
   if (fw_image_validate(image_buffer_ptr) != image_error_e::SUCCESS)
   {
      return fw_update_error_e::INVALID_IMAGE;
   }

   // 2. Find the location to write the image
   uint32_t *new_image_ptr;
   //uint32_t *old_image_ptr;

   // Start by checking the first image location
   fw_image_header_t *old_header = reinterpret_cast<fw_image_header_t *>(fw_image_A_ptr);
   fw_image_header_t *new_header = reinterpret_cast<fw_image_header_t *>(image_buffer_ptr);
   if (old_header->active)
   {
      // FW Image A is running, so we want to update B
      new_image_ptr = fw_image_B_ptr;
      //old_image_ptr = fw_image_A_ptr;
   }
   else
   {
      // Otherwise, we are running out of B and want to update A
      new_image_ptr = fw_image_A_ptr;
      //old_image_ptr = fw_image_B_ptr;
      old_header    = reinterpret_cast<fw_image_header_t *>(fw_image_B_ptr);
   }

   // 3. Write the new image to the new location.
   if (flash_write(reinterpret_cast<uint8_t *>(new_image_ptr), image_buffer_ptr, (new_header->image_size + sizeof(fw_image_header_t))) != flash_error_e::SUCCESS)
   {
      return fw_update_error_e::GEN_FAILURE;
   }
   
   // Clear the active flag in the header for the old image.
   if (flash_clear_byte(reinterpret_cast<uint8_t *>(old_header)) != flash_error_e::SUCCESS)
   {
      return fw_update_error_e::GEN_FAILURE;
   }

   return fw_update_error_e::SUCCESS;
}