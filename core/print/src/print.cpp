#include "print.h"
#include "stm32f7xx_hal_uart.h"

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