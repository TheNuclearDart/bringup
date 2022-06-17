#ifndef __UART_H
#define __UART_H

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

class UART
{
   public:
      UART(USART_TypeDef *uartInstance, uint32_t baudRate, uint32_t wordLength, uint32_t stopBits, uint32_t parity, uint32_t mode, uint32_t hwFlowCtl, uint32_t overSampling, uint32_t oneBitSampling, uint32_t timeout);
      UART(UART_HandleTypeDef &uart, uint32_t timeout); // Constructor. Should this take anything else? Do we even need this wrapper?
      void init(void);
      void out(const uint8_t &data, const uint32_t size);
      void out_raw(const uint8_t *pData, uint16_t Size, uint32_t Timeout);
   private:
      UART_HandleTypeDef uartInstance; // UART instance
      uint32_t timeout;
};

bool UART_is_initialized(void);

#endif