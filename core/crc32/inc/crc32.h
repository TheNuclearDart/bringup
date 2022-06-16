#ifndef __CRC32_H
#define __CRC32_H

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

class CRC32
{
   public:
      CRC32(void);
      void init(void);
      uint32_t calculate(uint32_t *buffer, uint32_t length);
   private:
      CRC_HandleTypeDef crc_instance;
};

uint32_t crc32_calculate(uint32_t *buffer, uint32_t length);
void     crc32_init(void);

#endif