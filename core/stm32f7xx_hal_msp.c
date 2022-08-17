/* @JDM code taken from STM32 example code */
// Need to try to move these into their lower level libs I guess?

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

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hltdc->Instance==LTDC)
  {
    /* Peripheral clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();
  
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOK_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    /**LTDC GPIO Configuration    
    PE4     ------> LTDC_B0
    PJ13     ------> LTDC_B1
    PK7     ------> LTDC_DE
    PK6     ------> LTDC_B7
    PK5     ------> LTDC_B6
    PG12     ------> LTDC_B4
    PJ14     ------> LTDC_B2
    PI10     ------> LTDC_HSYNC
    PK4     ------> LTDC_B5
    PJ15     ------> LTDC_B3
    PI9     ------> LTDC_VSYNC
    PK1     ------> LTDC_G6
    PK2     ------> LTDC_G7
    PI15     ------> LTDC_R0
    PJ11     ------> LTDC_G4
    PK0     ------> LTDC_G5
    PI14     ------> LTDC_CLK
    PJ8     ------> LTDC_G1
    PJ10     ------> LTDC_G3
    PJ7     ------> LTDC_G0
    PJ9     ------> LTDC_G2
    PJ6     ------> LTDC_R7
    PJ4     ------> LTDC_R5
    PJ5     ------> LTDC_R6
    PJ3     ------> LTDC_R4
    PJ2     ------> LTDC_R3
    PJ0     ------> LTDC_R1
    PJ1     ------> LTDC_R2 
    */
    GPIO_InitStruct.Pin = LCD_B0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(LCD_B0_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_B1_Pin|LCD_B2_Pin|LCD_B3_Pin|LCD_G4_Pin 
                          |LCD_G1_Pin|LCD_G3_Pin|LCD_G0_Pin|LCD_G2_Pin 
                          |LCD_R7_Pin|LCD_R5_Pin|LCD_R6_Pin|LCD_R4_Pin 
                          |LCD_R3_Pin|LCD_R1_Pin|LCD_R2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_DE_Pin|LCD_B7_Pin|LCD_B6_Pin|LCD_B5_Pin 
                          |LCD_G6_Pin|LCD_G7_Pin|LCD_G5_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_B4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(LCD_B4_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_HSYNC_Pin|LCD_VSYNC_Pin|LCD_R0_Pin|LCD_CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(LTDC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(LTDC_IRQn);

  }

}

void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc)
{
   if(hcrc->Instance==CRC)
   {
      /* Peripheral clock enable */
      __HAL_RCC_CRC_CLK_ENABLE();
   }
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
  //static DMA_HandleTypeDef dma_handle;
  GPIO_InitTypeDef gpio_init_structure;
  
  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();
  
  /* Enable chosen DMAx clock */
  //__DMAx_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;
  
  /* GPIOC configuration */
  gpio_init_structure.Pin   = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* GPIOD configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9 |
                              GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 |\
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 |\
                              GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_8 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOH configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_3 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure); 
  
  /* Configure common DMA parameters */
  //dma_handle.Init.Channel             = SDRAM_DMAx_CHANNEL;
  //dma_handle.Init.Direction           = DMA_MEMORY_TO_MEMORY;
  //dma_handle.Init.PeriphInc           = DMA_PINC_ENABLE;
  //dma_handle.Init.MemInc              = DMA_MINC_ENABLE;
  //dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  //dma_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  //dma_handle.Init.Mode                = DMA_NORMAL;
  //dma_handle.Init.Priority            = DMA_PRIORITY_HIGH;
  //dma_handle.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
  //dma_handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  //dma_handle.Init.MemBurst            = DMA_MBURST_SINGLE;
  //dma_handle.Init.PeriphBurst         = DMA_PBURST_SINGLE; 
  
  //dma_handle.Instance = SDRAM_DMAx_STREAM;
  
   /* Associate the DMA handle */
  //__HAL_LINKDMA(hsdram, hdma, dma_handle);
  
  /* Deinitialize the stream for new transfer */
  //HAL_DMA_DeInit(&dma_handle);
  
  /* Configure the DMA stream */
  //HAL_DMA_Init(&dma_handle); 
  
  /* NVIC configuration for DMA transfer complete interrupt */
  //HAL_NVIC_SetPriority(SDRAM_DMAx_IRQn, 5, 0);
  //HAL_NVIC_EnableIRQ(SDRAM_DMAx_IRQn); */
}