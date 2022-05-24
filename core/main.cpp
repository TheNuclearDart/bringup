/*************************************
 * Need to add a license or something
 ************************************/

#include <stdint.h>
#include <stdio.h>

#include "stm32f746xx.h"
#include "stm32f7xx_hal.h"


#include "print.h"
#include "usb_host.h"

// File local variables
namespace
{
   UART_HandleTypeDef uart1 = 
   {
      .Instance = USART1,
      .Init =
      {
         .BaudRate = 115200,
         .WordLength = UART_WORDLENGTH_8B,
         .StopBits = UART_STOPBITS_1,
         .Parity = UART_PARITY_NONE,
         .Mode = UART_MODE_TX_RX,
         .HwFlowCtl = UART_HWCONTROL_NONE,
         .OverSampling = UART_OVERSAMPLING_16,
         .OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE
      },
      .AdvancedInit =
      {
         .AdvFeatureInit = UART_ADVFEATURE_NO_INIT
      }
   };
   Print print(uart1, UINT32_MAX); // Need to determine better timeout. Is uart1 what I want?
   USB_Host usb;
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
   print.out(ch, 1);
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
      //Error_Handler();
   }
   /** Activate the Over-Drive mode 
    */
   if (HAL_PWREx_EnableOverDrive() != HAL_OK)
   {
      //Error_Handler();
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
      //Error_Handler();
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
      //Error_Handler();
   }
}

// Implementation of wrapper function for USB Host class. Not sure that this is the best way to do this.
void USB_Host::userFuncWrapper(USBH_HandleTypeDef *pHostHandle, uint8_t id)
{
   usb.userFunc(pHostHandle, id);
}

int main(void)
{
   HAL_Init(); // Init ST Provided HAL
   SystemClock_Config(); // Initialize system clocks

   usb.start(); // Start USB host. Was initialized at declaration

   while (1)
   {
      // Going to replace this whole idea with just a printf redirect, which is better. Probably reuse the class as something to wrap the hw for printf
      uint8_t data[] = "abc";
      //data[1] = 'B';
      //data[2] = '\r';
      //data[3] = '\n';
      print.outRaw(data, 3, UINT32_MAX);
      //print.out((uint8_t)data[1], 1);
      //print.out((uint8_t)data[2], 1);
      //print.out((uint8_t)data[3], 1);
      //printf("Testing, I am a computer \n");
      HAL_Delay(1000);
   }

   return 1;
}