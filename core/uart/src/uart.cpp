#include <stdio.h>

#include "uart.h"

bool initialized = false;

/**
 * @brief Simple wrapper for setting up UART to print. Not sure this is the best way to do this
 * 
 */

UART::UART(UART_HandleTypeDef &uart, uint32_t timeout)
{
   this->uartInstance = uart;
   this->timeout = timeout;
}

UART::UART(USART_TypeDef *uartInstance, uint32_t baudRate, uint32_t wordLength, uint32_t stopBits, uint32_t parity, uint32_t mode, uint32_t hwFlowCtl, uint32_t overSampling, uint32_t oneBitSampling, uint32_t timeout)
{
   if (initialized)
   {
      printf("Print library already initialized...\r\n");
      return;
   }
   else
   {
      initialized = true;
   
      this->uartInstance = 
      {
         .Instance = uartInstance,
         .Init =
         {
            .BaudRate = baudRate,
            .WordLength = wordLength,
            .StopBits = stopBits,
            .Parity = parity,
            .Mode = mode,
            .HwFlowCtl = hwFlowCtl,
            .OverSampling = overSampling,
            .OneBitSampling = oneBitSampling
         },
         .AdvancedInit =
         {
            .AdvFeatureInit = UART_ADVFEATURE_NO_INIT
         }
      };
      this->timeout = timeout;
   }
}

void UART::init(void)
{
   HAL_UART_Init(&this->uartInstance);
}

void UART::out(const uint8_t &data, const uint32_t size)
{
   HAL_UART_Transmit(&this->uartInstance, &data, size, this->timeout);
}

// Raw wrapper for HAL_UART_Transmit
void UART::out_raw(const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
   HAL_UART_Transmit(&this->uartInstance, pData, Size, Timeout);
}

UART::error UART::receive_char(uint8_t &char_buff)
{
   if(HAL_UART_Receive(&this->uartInstance, &char_buff, 1, this->timeout) == HAL_TIMEOUT)
   {
      return error::TIMEOUT;
   }

   return error::OKAY;
}

bool UART_is_initialized(void)
{
   return initialized;
}