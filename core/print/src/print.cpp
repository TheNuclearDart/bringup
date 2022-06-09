#include <stdio.h>

#include "print.h"

bool initialized = false;

/**
 * @brief Simple wrapper for setting up UART to print. Not sure this is the best way to do this
 * 
 */

Print::Print(UART_HandleTypeDef &uart, uint32_t timeout)
{
   this->uartInstance = uart;
   this->timeout = timeout;
}

Print::Print(USART_TypeDef *uartInstance, uint32_t baudRate, uint32_t wordLength, uint32_t stopBits, uint32_t parity, uint32_t mode, uint32_t hwFlowCtl, uint32_t overSampling, uint32_t oneBitSampling, uint32_t timeout)
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

void Print::init(void)
{
   HAL_UART_Init(&this->uartInstance);
}

void Print::out(const uint8_t &data, const uint32_t size)
{
   HAL_UART_Transmit(&this->uartInstance, &data, size, this->timeout);
}

// Raw wrapper for HAL_UART_Transmit
void Print::out_raw(const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
   HAL_UART_Transmit(&this->uartInstance, pData, Size, Timeout);
}

bool print_is_initialized(void)
{
   return initialized;
}