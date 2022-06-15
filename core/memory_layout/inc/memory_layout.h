#include <stdint.h>

extern uint32_t __app_start__;
extern uint32_t __app_size__;
extern uint32_t __sram_start__;

const uint32_t fw_image_size = (reinterpret_cast<uint32_t>(&__app_size__)) / 2;
const uint32_t *fw_image_A_ptr = &__app_start__;
const uint32_t *fw_image_B_ptr = fw_image_A_ptr + fw_image_size;