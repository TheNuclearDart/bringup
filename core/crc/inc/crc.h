#ifndef __CRC_H
#define __CRC_H

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

class CRC_Handler
{
   public:
      enum class mode
      {
         BIT16,
         BIT32
      };
      CRC_Handler(void);
      void init(void);
      uint32_t calculate(uint32_t *buffer, uint32_t length, mode crc_mode);
   private:
      CRC_HandleTypeDef crc_instance;
      mode crc_mode;
      void change_mode(mode mode);
};

uint32_t crc32_calculate(uint32_t *buffer, uint32_t length);
uint32_t crc_calculate(uint32_t *buffer, uint32_t length, CRC_Handler::mode crc_mode);
void     crc_init(void);

#endif