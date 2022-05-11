#ifndef __PRINT_H
#define __PRINT_H

#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_uart.h"

class Print
{
   public:
      Print(UART_HandleTypeDef &uart, uint32_t timeout); // Constructor. Should this take anything else? Do we even need this wrapper?
      void out(const uint8_t &data, const uint32_t size);
   private:
      UART_HandleTypeDef uartInstance; // UART instance
      uint32_t timeout;
};

#endif