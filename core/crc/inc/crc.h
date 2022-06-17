#ifndef __CRC_H
#define __CRC_H

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

class CRC_Handler
{
   public:
      CRC_Handler(void);
      void init(void);
      uint32_t calculate(uint32_t *buffer, uint32_t length);
   private:
      CRC_HandleTypeDef crc_instance;
};

uint32_t crc_calculate(uint32_t *buffer, uint32_t length);
void     crc_init(void);

#endif