#ifndef __LCD_H
#define __LCD_H

#include "stm32f7xx.h"

class LCD
{
   public:
      enum class color_depths
      {
         COLOR_DEPTH_8,
         COLOR_DEPTH_16,
      };
      LCD(uint32_t horizontal_res, uint32_t vertical_res, color_depths color_depth);
      void init(uint32_t frame_buffer_address);
      void config_layer(LTDC_LayerCfgTypeDef *layer_cfg); // Will likely change
      void flush();
      uint32_t get_horizontal_res(void);
      uint32_t get_vertical_res(void);
      color_depths get_color_depth(void);
      void set_pixel(uint32_t x, uint32_t y, uint16_t color);

   private:
      LTDC_HandleTypeDef ltdcInstance;
      uint32_t           horizontal_res;
      uint32_t           vertical_res;
      color_depths       color_depth;
      uint32_t           frame_buffer_address;
};

#endif