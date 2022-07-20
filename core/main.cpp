/*************************************
 * Need to add a license or something
 ************************************/

#include <stdint.h>
#include <stdio.h>

#include "stm32f746xx.h"
#include "stm32f7xx_hal.h"
#include "syscalls.h"

#include "crc.h"
#include "fw_header.h"
#include "fw_image.h"
#include "fw_update.h"
#include "gpio_defines.h"
#include "memory_layout.h" // Temporary
#include "lcd.h"
#include "uart.h"
#include "usb_host.h"

// File local variables
namespace
{
   // Need to decouple this more from the HAL
   UART uart(USART1, 115200, UART_WORDLENGTH_8B, UART_STOPBITS_1, UART_PARITY_NONE, UART_MODE_TX_RX, UART_HWCONTROL_NONE, UART_OVERSAMPLING_16, UART_ONE_BIT_SAMPLE_DISABLE, 1000);
   LCD lcd;
   uint8_t fw_image_buffer[0x10000]; // Arbitrary size, but it's going to need to be bigger (and likely in external RAM) once the image gets bigger
   //USB_Host usb;
}

// These need to live in a separate file, not sure where yet, maybe something with print.cpp
// Also not sure how to use them with a single instance of Print
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
   uart.out(ch, 1);
   return ch;
}

// Taken from cubemx generated code. Should try to understand.
void SystemClock_Config(void)
{
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
   RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

   /** Configure LSE Drive Capability 
    */
   HAL_PWR_EnableBkUpAccess();
   /** Configure the main internal regulator output voltage 
    */
   __HAL_RCC_PWR_CLK_ENABLE();
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
   /** Initializes the CPU, AHB and APB busses clocks 
    */
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
   RCC_OscInitStruct.LSIState = RCC_LSI_ON;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLM = 25;
   RCC_OscInitStruct.PLL.PLLN = 400;
   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
   RCC_OscInitStruct.PLL.PLLQ = 9;
   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
   {
      assert_param(0);
   }
   /** Activate the Over-Drive mode 
    */
   if (HAL_PWREx_EnableOverDrive() != HAL_OK)
   {
      assert_param(0);
   }
   /** Initializes the CPU, AHB and APB busses clocks 
    */
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
   {
      assert_param(0);
   }
   PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPDIFRX|RCC_PERIPHCLK_LTDC
                                 |RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                                 |RCC_PERIPHCLK_USART6|RCC_PERIPHCLK_SAI2
                                 |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C3
                                 |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
   PeriphClkInitStruct.PLLI2S.PLLI2SN = 100;
   PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLP_DIV2;
   PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
   PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
   PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
   PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
   PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
   PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
   PeriphClkInitStruct.PLLI2SDivQ = 1;
   PeriphClkInitStruct.PLLSAIDivQ = 1;
   PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
   PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
   PeriphClkInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI;
   PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
   PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
   PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
   PeriphClkInitStruct.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
   PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
   PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
   if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
   {
      assert_param(0);
   }
}

// Implementation of wrapper function for USB Host class. Not sure that this is the best way to do this.
void USB_Host::userFuncWrapper(USBH_HandleTypeDef *pHostHandle, uint8_t id)
{
   //usb.userFunc(pHostHandle, id);
}

/**
  * @brief GPIO Initialization Function @JDM band-aid copied function for now...
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, ARDUINO_D7_Pin|ARDUINO_D8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DCMI_PWR_EN_GPIO_Port, DCMI_PWR_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, ARDUINO_D4_Pin|ARDUINO_D2_Pin|EXT_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : OTG_HS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_HS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_HS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ULPI_D7_Pin ULPI_D6_Pin ULPI_D5_Pin ULPI_D3_Pin 
                           ULPI_D2_Pin ULPI_D1_Pin ULPI_D4_Pin */
  GPIO_InitStruct.Pin = ULPI_D7_Pin|ULPI_D6_Pin|ULPI_D5_Pin|ULPI_D3_Pin 
                          |ULPI_D2_Pin|ULPI_D1_Pin|ULPI_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_VBUS_Pin */
  GPIO_InitStruct.Pin = OTG_FS_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_VBUS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Audio_INT_Pin */
  GPIO_InitStruct.Pin = Audio_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Audio_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARDUINO_D7_Pin ARDUINO_D8_Pin LCD_DISP_Pin */
  GPIO_InitStruct.Pin = ARDUINO_D7_Pin|ARDUINO_D8_Pin|LCD_DISP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : uSD_Detect_Pin */
  GPIO_InitStruct.Pin = uSD_Detect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(uSD_Detect_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_BL_CTRL_Pin */
  GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TP3_Pin NC2_Pin */
  GPIO_InitStruct.Pin = TP3_Pin|NC2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : DCMI_PWR_EN_Pin */
  GPIO_InitStruct.Pin = DCMI_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DCMI_PWR_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_INT_Pin */
  GPIO_InitStruct.Pin = LCD_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LCD_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ULPI_NXT_Pin */
  GPIO_InitStruct.Pin = ULPI_NXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(ULPI_NXT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARDUINO_D4_Pin ARDUINO_D2_Pin EXT_RST_Pin */
  GPIO_InitStruct.Pin = ARDUINO_D4_Pin|ARDUINO_D2_Pin|EXT_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : ULPI_STP_Pin ULPI_DIR_Pin */
  GPIO_InitStruct.Pin = ULPI_STP_Pin|ULPI_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : RMII_RXER_Pin */
  GPIO_InitStruct.Pin = RMII_RXER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RMII_RXER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ULPI_CLK_Pin ULPI_D0_Pin */
  GPIO_InitStruct.Pin = ULPI_CLK_Pin|ULPI_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // @JDM configure SCK/D13 pin (has ld1 attached)
  GPIO_InitStruct.Pin = ARDUINO_SCK_D13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ARDUINO_SCK_D13_GPIO_Port, &GPIO_InitStruct);

  // Setup user Push Button, to be used as a trigger for fw update
  GPIO_InitStruct.Pin = USER_PUSH_BUTTON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(USER_PUSH_BUTTON_PORT, &GPIO_InitStruct);
}

int main(void)
{
   HAL_Init(); // Init ST Provided HAL
   SystemClock_Config(); // Initialize system clocks
   MX_GPIO_Init(); // This needs replacing or fixing, wasn't needed for UART like thought

   uart.init();
   printf("Print initialized in main app!\r\n");
   printf("FW Version: %lx\r\nCRC: %lx\r\n", fw_header_get_current_header()->fw_version, fw_header_get_current_header()->crc32);
   // Check the image header and print which image we're running out of
   fw_image_header_t *header_a = reinterpret_cast<fw_image_header_t *>(fw_image_A_ptr);
   fw_image_header_t *header_b = reinterpret_cast<fw_image_header_t *>(fw_image_B_ptr);
   printf("FLASH Header Active: \r\nA: %lX\r\nB: %lX\r\n", (uint32_t)header_a->active, (uint32_t)header_b->active);
   //usb.start(); // Start USB host. Was initialized at declaration

   lcd.init();
   crc_init();
   fw_image_init(&crc32_calculate); // I feel like I need a different solution for this.
   printf("LCD initialized!\r\n");

   printf("Initialization complete, beginning main loop\r\n");
   printf("fw_image_A_ptr = %lX\r\n", (uint32_t)fw_image_A_ptr);
   printf("fw_image_B_ptr = %lX\r\n", (uint32_t)fw_image_B_ptr);
   printf("image size = %lX\r\n", fw_image_size);
   printf("This is a new image!! Again!!\r\n");

   while (1)
   {
      // All of this should be in its own thread once we have the OS??
      GPIO_PinState read_val = HAL_GPIO_ReadPin(USER_PUSH_BUTTON_PORT, USER_PUSH_BUTTON_PIN);
      if (read_val == GPIO_PIN_SET)
      {
         printf("FW Update Process Triggered.\r\n");
         // This is broken and i don't know why, main image doesn't boot with these uncommented.
         UART::error error = uart.xmodem_receive(fw_image_buffer, 0x10000);
         HAL_Delay(1000); // Temporary
         if (error == UART::error::TIMEOUT)
         {
            printf("XModem timed out!\r\n");
         }
         else if (error == UART::error::OKAY)
         {
            printf("Image downloaded successfully! Writing image to destination in flash...\r\n");
         }
         fw_update_error_e update_error = fw_update(fw_image_buffer);
         if (update_error != fw_update_error_e::SUCCESS)
         {
            printf("FW Update failed!\r\n");
         }
         else
         {
            printf("FW Update succeeded! Resetting Device...\r\n");
            NVIC_SystemReset();
         }
      }
      //usb.process();
   }

   return 1;
}           