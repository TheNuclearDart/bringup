#include "print.h"
#include "stm32f7xx.h"

/**
 * @brief Simple wrapper for setting up UART to print. Not sure this is the best way to do this
 * 
 */

Print::Print(UART_HandleTypeDef &uart, uint32_t timeout)
{
   this->uartInstance = uart;
   this->timeout = timeout;

   HAL_UART_Init(&this->uartInstance);
}

void Print::out(const uint8_t &data, const uint32_t size)
{
   HAL_UART_Transmit(&this->uartInstance, &data, size, this->timeout);
}

// Raw wrapper for HAL_UART_Transmit
void Print::outRaw(const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
   HAL_UART_Transmit(&this->uartInstance, pData, Size, Timeout);
}