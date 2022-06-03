#include "lcd.h"
#include "stm32f7xx.h"

#include <stdio.h>

// Only basic init in this file for now in order to get the screen off of a bright white, needs filling out to do anything

static bool initialized = false;

LCD::LCD(void)
{
   // Values taken from CubeMX generated code
   this->ltdcInstance = 
   {
      .Instance = LTDC,
      .Init = 
      {
         .HSPolarity = LTDC_HSPOLARITY_AL,
         .VSPolarity = LTDC_VSPOLARITY_AL,
         .DEPolarity = LTDC_DEPOLARITY_AL,
         .PCPolarity = LTDC_PCPOLARITY_IPC,
         .HorizontalSync = 40, // Need to understand these values.
         .VerticalSync = 9,
         .AccumulatedHBP = 53,
         .AccumulatedVBP = 11,
         .AccumulatedActiveW = 533,
         .AccumulatedActiveH = 283,
         .TotalWidth = 565,
         .TotalHeigh = 285,
         .Backcolor =
         {
            .Blue = 0,
            .Green = 0,
            .Red = 0,
         }
      }
   };
}

void LCD::init(void)
{
   if (initialized)
   {
      // Assert? Do something
      printf("This library already initialized.");
   }
   else
   {
      initialized = true;
      if (HAL_LTDC_Init(&this->ltdcInstance) != HAL_OK)
      {
         assert_msg(0, "Error in HAL_LTDC_Init!\r\n");
      }

      LTDC_LayerCfgTypeDef layerCfg = 
      {
         .WindowX0 = 0,
         .WindowX1 = 480,
         .WindowY0 = 0,
         .WindowY1 = 272,
         .PixelFormat = LTDC_PIXEL_FORMAT_RGB565,
         .Alpha = 255,
         .Alpha0 = 0,
         .BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA,
         .BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA,
         .FBStartAdress = 0xC0000000,
         .ImageWidth = 480,
         .ImageHeight = 272,
         .Backcolor =
         {
            .Blue = 0,
            .Green = 0,
            .Red = 0
         }
      };
      if (HAL_LTDC_ConfigLayer(&this->ltdcInstance, &layerCfg, 0) != HAL_OK)
      {
         assert_msg(0, "Error in HAL_LTDC_ConfigLayer!\r\n");
      }
   }
}

