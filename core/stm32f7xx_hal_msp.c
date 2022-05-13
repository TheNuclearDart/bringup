/* @JDM code taken from STM32 example code */

#include "gpio_defines.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_tim.h"

void HAL_MspInit(void)
{
   __HAL_RCC_PWR_CLK_ENABLE();
   __HAL_RCC_SYSCFG_CLK_ENABLE();

   HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0); // Purpose??
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
   // Borrowed from generated code. Likely needs some work
   if (htim_base->Instance == TIM1)
   {
      __HAL_RCC_TIM1_CLK_ENABLE();
   }
   else if (htim_base->Instance == TIM2)
   {
      __HAL_RCC_TIM2_CLK_ENABLE();
   }
   else if (htim_base->Instance == TIM3)
   {
      __HAL_RCC_TIM3_CLK_ENABLE();
   }
   else if (htim_base->Instance == TIM5)
   {
      __HAL_RCC_TIM5_CLK_ENABLE();
   }
   else if (htim_base->Instance == TIM8)
   {
      __HAL_RCC_TIM8_CLK_ENABLE();
   }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base)
{
   if(htim_base->Instance == TIM1)
   {
      /* Peripheral clock disable */
      __HAL_RCC_TIM1_CLK_DISABLE();
   }
   else if (htim_base->Instance == TIM2)
   {
      /* Peripheral clock disable */
      __HAL_RCC_TIM2_CLK_DISABLE();
   }
   else if (htim_base->Instance == TIM3)
   {
      /* Peripheral clock disable */
      __HAL_RCC_TIM3_CLK_DISABLE();
   }
   else if (htim_base->Instance == TIM5)
   {
      /* Peripheral clock disable */
      __HAL_RCC_TIM5_CLK_DISABLE();
   }
  else if (htim_base->Instance == TIM8)
  {
      /* Peripheral clock disable */
      __HAL_RCC_TIM8_CLK_DISABLE();
  }
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