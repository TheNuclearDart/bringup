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
      printf("This library already initialized.\r\n");
   }
   else
   {
      initialized = true;
      if (HAL_LTDC_Init(&this->ltdcInstance) != HAL_OK)
      {
         assert_msg(0, "Error in HAL_LTDC_Init!\r\n");
      }
   }
}

void LCD::config_layer(LTDC_LayerCfgTypeDef *layer_cfg)
{
   if (HAL_LTDC_ConfigLayer(&this->ltdcInstance, layer_cfg, 0) != HAL_OK)
   {
      assert_param(0);
   }
}
