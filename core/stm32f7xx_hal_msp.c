/* @JDM code taken from STM32 example code */

#include "gpio_defines.h"
#include "stm32f746xx.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_gpio.h"
#include "stm32f7xx_hal_gpio_ex.h"
#include "stm32f7xx_hal_uart.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_rcc_ex.h"

void HAL_MspInit(void)
{
   __HAL_RCC_PWR_CLK_ENABLE();
   __HAL_RCC_SYSCFG_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
   // USART1 is the Virtual COM Port
   if (huart->Instance == USART1)
   {
      GPIO_InitTypeDef GPIO_InitStruct = {0};

      __HAL_RCC_USART1_CLK_ENABLE(); // Enable peripheral clock
      __HAL_RCC_GPIOB_CLK_ENABLE();
      __HAL_RCC_GPIOA_CLK_ENABLE();

      // VCP_TX = PA9
      // VCP_RX = PB7
      GPIO_InitStruct.Pin = VCP_RX_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
      HAL_GPIO_Init(VCP_RX_GPIO_Port, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = VCP_TX_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
      HAL_GPIO_Init(VCP_TX_GPIO_Port, &GPIO_InitStruct);
   }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
   if (huart->Instance == USART1)
   {
      __HAL_RCC_USART1_CLK_DISABLE(); // Disable peripheral clock

      HAL_GPIO_DeInit(VCP_RX_GPIO_Port, VCP_RX_Pin); // Deinit pins
      HAL_GPIO_DeInit(VCP_TX_GPIO_Port, VCP_TX_Pin);
   }

}