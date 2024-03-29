#include "lcd.h"
#include "stm32f7xx.h"

#include <stdio.h>

// Only basic init in this file for now in order to get the screen off of a bright white, needs filling out to do anything

namespace
{
   bool initialized = false;
}

LCD::LCD(uint32_t horizontal_res, uint32_t vertical_res, color_depths color_depth)
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

   this->color_depth    = color_depth;
   this->horizontal_res = horizontal_res;
   this->vertical_res   = vertical_res;
}

void LCD::init(uint32_t frame_buffer_address)
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
      this->frame_buffer_address = frame_buffer_address;
      LTDC_LayerCfgTypeDef layer_cfg = 
      {
         .WindowX0 = 0,
         .WindowX1 = 480,
         .WindowY0 = 0,
         .WindowY1 = 272,
         .PixelFormat = LTDC_PIXEL_FORMAT_RGB888, // Make this an option
         .Alpha = 255,
         .Alpha0 = 0,
         .BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA,
         .BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA,
         .FBStartAdress = this->frame_buffer_address,
         .ImageWidth = 480,
         .ImageHeight = 272,
         .Backcolor =
         {
            .Blue = 0,
            .Green = 0,
            .Red = 0
         }
      };
      this->config_layer(&layer_cfg);
   }
}

void LCD::config_layer(LTDC_LayerCfgTypeDef *layer_cfg)
{
   if (HAL_LTDC_ConfigLayer(&this->ltdcInstance, layer_cfg, 0) != HAL_OK)
   {
      assert_param(0);
   }
}

LCD::color_depths LCD::get_color_depth(void)
{
   return this->color_depth;
}

uint32_t LCD::get_horizontal_res(void)
{
   return this->horizontal_res;
}

uint32_t LCD::get_vertical_res(void)
{
   return this->vertical_res;
}

void LCD::flush(void)
{
   HAL_LTDC_Reload(&this->ltdcInstance, LTDC_RELOAD_VERTICAL_BLANKING);
}

void LCD::set_pixel(uint32_t x, uint32_t y, lcd_pixel_t color)
{
   // (y * horizontal_res) + x = index into framebuffer to set
   uint32_t index = (y * this->horizontal_res) + x;
   
   switch(this->color_depth)
   {
      case color_depths::COLOR_DEPTH_24:
      {
         lcd_pixel_t *frame_buffer = reinterpret_cast<lcd_pixel_t *>(this->frame_buffer_address);
         frame_buffer[index] = color; 
         break;
      }
      case color_depths::COLOR_DEPTH_16:
      {
         uint16_t *frame_buffer = reinterpret_cast<uint16_t *>(this->frame_buffer_address);
         //fixmeframe_buffer[index]    = color;
         break;
      }
      case color_depths::COLOR_DEPTH_8:
      {
         uint8_t *frame_buffer = reinterpret_cast<uint8_t *>(this->frame_buffer_address);
         //fixmeframe_buffer[index]   = color;
         break;
      }
      default:
         assert_param(0);
   }
}