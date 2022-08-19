#include <cstring>
#include <stdio.h>

#include "stm32f7xx.h"

#include "fw_header.h"
#include "fw_image.h"
#include "memory_layout.h"
#include "shared_data.h"

typedef struct _image_info_t
{
   uint32_t *image_src_ptr;
   uint32_t *image_dest_ptr;
   uint32_t image_size;
} image_info_t;

// local functions
static void          relocate_image(uint8_t *image_ptr, uint32_t image_size);
static uint32_t      handle_segment(segment_header_t *segment_addr);
static image_error_e find_validate_image(uint32_t *image_addr);

namespace 
{
   // local data
   image_info_t image_info = {};
   uint32_t (*crc32_func)(uint32_t *buffer, uint32_t length); // Function pointer for calculating CRC
   int (*print_func)(const char *format, ...);
}

void fw_image_init(uint32_t (*crc32_func_ptr)(uint32_t *buffer, uint32_t length), int (*print_func_ptr)(const char *format, ...))
{
   crc32_func = crc32_func_ptr;
   print_func = print_func_ptr;
}

// Validate, relocate/load, and execute
image_error_e fw_image_load(void)
{
   uint32_t *image_region_ptr = &__app_start__;

   image_error_e error = find_validate_image(image_region_ptr);
   if (error != image_error_e::SUCCESS)
   {
      return error;
   }

   relocate_image(reinterpret_cast<uint8_t *>(image_info.image_src_ptr), image_info.image_size); // Copy everything including header

   return image_error_e::SUCCESS;
}

/**
 * @brief Relocates image to given destination. For now a simple memcpy
 * 
 * @param image_addr location of image
 * @param dest_addr destination for image (SRAM, etc)
 */
static void relocate_image(uint8_t *image_ptr, uint32_t size)
{
   uint32_t total_bytes = sizeof(fw_image_header_t);
   segment_header_t *segment_header = reinterpret_cast<segment_header_t *>(image_ptr + sizeof(fw_image_header_t));
   
   do
   {
      uint32_t bytes_moved = handle_segment(segment_header);
      total_bytes += (bytes_moved + sizeof(segment_header_t));
      segment_header = reinterpret_cast<segment_header_t *>(reinterpret_cast<uint8_t *>(segment_header) + sizeof(segment_header_t) + bytes_moved);
   } while (total_bytes != size);
}

static uint32_t handle_segment(segment_header_t *segment_header)
{
   uint32_t segment_size = segment_header->size;
   uint8_t *segment_dest = reinterpret_cast<uint8_t *>(segment_header->dest_addr);
   uint8_t *segment_src  = reinterpret_cast<uint8_t *>(segment_header) + sizeof(segment_header_t);

   memcpy(segment_dest, segment_src, segment_size);

   return segment_size;
}

/**
 * @brief Finds valid images in given region, and returns pointer and size
 * 
 * @param image_addr ptr to region containing images
 * @param image_info struct to be filled out with valid image info
 * @return image_error_e error indicating result
 */
static image_error_e find_validate_image(uint32_t *image_ptr)
{
   // Check the header for image A
   fw_image_header_t *header_a = reinterpret_cast<fw_image_header_t *>(image_ptr);
   fw_image_header_t *header_b = reinterpret_cast<fw_image_header_t *>(fw_image_B_ptr);
   
   bool fallback = false;

   if (header_a->active)
   {
      // Calculate CRC
      image_error_e error = fw_image_validate(reinterpret_cast<uint8_t *>(image_ptr));

      if (error != image_error_e::SUCCESS)
      {
         print_func("Image A validation failed! Falling back on B...\r\n");
         fallback = true; // The image was bad, so we need to fallback on the other image, even though this one was active
      }

      if (!fallback)
      {
         print_func("Image A is valid!\r\n");
         // Assign values to global struct
         image_info.image_src_ptr = reinterpret_cast<uint32_t *>(image_ptr);
         image_info.image_size = header_a->image_size;
         image_info.image_dest_ptr = reinterpret_cast<uint32_t *>(header_a->entry_addr);
         // Populate the shared data SRAM region with active header
         shared_data_populate_active_fw_header(header_a);
      }
   }
   // Now check image B's header if the image is active, or we're falling back on it.
   else if (header_b->active || fallback == true)
   {
      image_error_e error = fw_image_validate(reinterpret_cast<uint8_t *>(header_b));

      if (error != image_error_e::SUCCESS)
      {
         print_func("Image B validation failed!\r\n");
         switch(header_b->active)
         {
            case false:
               if (fallback)
               {
                  // Since we're already falling back on this image, A must also be bad, so completely fail out.
                  // In the future, if we get there, we could fall into a recovery image.
                  return image_error_e::NO_VALID_IMAGES;
               }
               break;
            case true:
               print_func("Falling back to image A...\r\n");
               // Since this image is invalid, we want to fallback to A
               fallback = true;
               break;
         }
      }
      
      if (!fallback)
      {
         print_func("Image B is valid!\r\n");
         image_info.image_src_ptr = reinterpret_cast<uint32_t *>(header_b);
         image_info.image_size = header_b->image_size;
         image_info.image_dest_ptr = reinterpret_cast<uint32_t *>(header_b->entry_addr);

         // Populate the shared data SRAM region with active header
         shared_data_populate_active_fw_header(header_b);
      }
   }
   // B was active but failed, so fallback on A
   else if (fallback)
   {
      image_error_e error = fw_image_validate(reinterpret_cast<uint8_t *>(image_ptr));

      if (error != image_error_e::SUCCESS)
      {
         print_func("Image A is invalid!\r\n");
         return image_error_e::NO_VALID_IMAGES;
      }

      // Assign values to global struct
      image_info.image_src_ptr = reinterpret_cast<uint32_t *>(image_ptr);
      image_info.image_size = header_a->image_size;
      image_info.image_dest_ptr = reinterpret_cast<uint32_t *>(header_a->entry_addr);

      // Populate the shared data SRAM region with active header
      shared_data_populate_active_fw_header(header_a);
   }

   return image_error_e::SUCCESS;
}

/**
 * @brief Excute image at give pointer
 * 
 * @param image_ptr 
 */
void fw_image_execute(void)
{
   // Set stack pointer to start of image. Image _should_ reset it to what it desires in it's own startup
   uint8_t *new_ptr = reinterpret_cast<uint8_t *>(image_info.image_dest_ptr);
   
   __set_MSP(reinterpret_cast<uint32_t>(new_ptr)); // The first word after the header is the address of the end of the stack.
   
   // Set vector table register to use Application's vector table
   __disable_irq();
   SCB->VTOR = (reinterpret_cast<uint32_t>(new_ptr));
   //__enable_irq();

   // Set function pointer to point to second full word, which is the application reset_handler
   uint32_t *new_app_start_ptr = reinterpret_cast<uint32_t *>(new_ptr); // Cast back to a uint32_t pointer
   uint32_t *app_start_ptr = reinterpret_cast<uint32_t *>(new_app_start_ptr[1]); // This points to the second word of the app (after the fw header),
                                                                                 // which contains the address of the reset_handler.
   void (*application)();
   application = (void (*)())app_start_ptr;
   
   // Start application!
   application();
}

/**
 * @brief Validate an image in a given buffer by checking its CRC
 * 
 * @param image_buffer_ptr Pointer to buffer containing image. This must point at the header of the image.
 * @return image_error_e Result of validation.
 */
image_error_e fw_image_validate(uint8_t *image_header_ptr)
{
   fw_image_header_t *header = reinterpret_cast<fw_image_header_t *>(image_header_ptr);

   print_func("CRC of incoming image in header is: %lx\r\n", header->crc32);
   uint32_t *actual_image_ptr = reinterpret_cast<uint32_t *>(image_header_ptr + sizeof(fw_image_header_t));
   uint32_t actual_image_size = header->image_size - sizeof(fw_image_header_t);
   uint32_t crc = crc32_func(actual_image_ptr, actual_image_size);
   print_func("CRC of incoming image calculated as: %lx\r\n", crc);

   if (crc != header->crc32)
   {
      print_func("CRC check failed!\r\n");
      return image_error_e::INVALID_IMAGE;
   }
   else
   {
      print_func("CRC check passed!\r\n");
      return image_error_e::SUCCESS;
   }
}