#ifndef __MEMORY_LAYOUT_H
#define __MEMORY_LAYOUT_H

#include <stdint.h>

extern uint32_t __app_start__;
extern uint32_t __app_size__;
extern uint32_t __sram_start__;
extern uint32_t __shared_data_start__;
extern uint32_t __shared_data_end__;

constexpr uint32_t fw_image_size = 0x70000; // 256KB for now, not sure that this will work forever. Currently only have 240k reserved in memory map (as that is max SRAM size)
                                            // This actually doesn't work, as the bootloader occupies the first sector of flash.  
constexpr uint32_t *fw_image_A_ptr = &__app_start__;
constexpr uint32_t *fw_image_B_ptr = fw_image_A_ptr + (fw_image_size / 4); // Incrementing by words, so divide by 4 for the correct number
const uint32_t shared_data_size = &__shared_data_end__ - &__shared_data_start__;

#endif