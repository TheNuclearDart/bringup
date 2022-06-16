#include <cstring>

#include "stm32f746xx.h"

#include "fw_header.h"
#include "fw_image.h"
#include "memory_layout.h"

typedef struct _image_info_t
{
   uint32_t *image_src_ptr;
   uint32_t *image_dest_ptr;
   uint32_t image_size;
} image_info_t;


// local functions
static void          relocate_image(uint32_t *image_ptr, uint32_t *dest_ptr, uint32_t image_size);
static image_error_e find_validate_image(uint32_t *image_addr);

// local data
image_info_t image_info = {0};

// Validate, relocate/load, and execute
image_error_e fw_image_load(void)
{
   uint32_t *image_region_ptr = &__app_start__;

   image_error_e error = find_validate_image(image_region_ptr);
   if (error != image_error_e::SUCCESS)
   {
      return error;
   }

   relocate_image(image_info.image_src_ptr, image_info.image_dest_ptr, image_info.image_size);

   return image_error_e::SUCCESS;
}

/**
 * @brief Relocates image to given destination. For now a simple memcpy
 * 
 * @param image_addr location of image
 * @param dest_addr destination for image (SRAM, etc)
 */
static void relocate_image(uint32_t *image_ptr, uint32_t *dest_ptr, uint32_t size)
{
   memcpy(dest_ptr, image_ptr, size);
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
   fw_image_header_t *header = reinterpret_cast<fw_image_header_t *>(image_ptr);

#if 1
   if (header->active)
   {
      //uint32_t header_size = fw_header_get_header_size();
      uint8_t *new_ptr = reinterpret_cast<uint8_t *>(image_ptr);
      //new_ptr += header_size;

      // Calculate CRC

      // Assign values to global struct
      image_info.image_src_ptr = reinterpret_cast<uint32_t *>(new_ptr);
      //image_info.image_size = header->image_size;
      image_info.image_size = reinterpret_cast<uint32_t>(&__app_size__); // Temporary until I can get tool filling in header
      
      uint32_t *sram_start_ptr = &__sram_start__; // Need to think about this, maybe the image header or something should have the start addr?
      image_info.image_dest_ptr = sram_start_ptr;

      return image_error_e::SUCCESS;
   }
   else
   {
      header = (header + fw_image_size);
      if (header->active)
      {
         image_info.image_src_ptr = const_cast<uint32_t *>(fw_image_B_ptr + fw_header_get_header_size());
         image_info.image_size = header->image_size;
      }
   }
#else
   // For now, just set everything to start of region and size of region
   image_info.image_ptr = image_ptr;
   image_info.image_size = reinterpret_cast<uint32_t>(&__app_size__);
#endif
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
   new_ptr += fw_header_get_header_size(); // Step over the fw header
   
   __set_MSP(reinterpret_cast<uint32_t>(new_ptr)); // The first word after the header is the address of the end of the stack.
   
   // Set vector table register to use Application's vector table
   __disable_irq();
   SCB->VTOR = (reinterpret_cast<uint32_t>(new_ptr));
   __enable_irq();

   // Set function pointer to point to second full word, which is the application reset_handler
   uint32_t *new_app_start_ptr = reinterpret_cast<uint32_t *>(new_ptr); // Cast back to a uint32_t pointer
   uint32_t *app_start_ptr = reinterpret_cast<uint32_t *>(new_app_start_ptr[1]); // This points to the second word of the app (after the fw header),
                                                                                 // which contains the address of the reset_handler.
   void (*application)();
   application = (void (*)())app_start_ptr;
   
   // Start application!
   application();
}