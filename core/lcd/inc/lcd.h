#ifndef __LCD_H
#define __LCD_H

#include "stm32f7xx.h"

typedef struct _lcd_pixel_t
{
   uint8_t blue;
   uint8_t green;
   uint8_t red;
   uint8_t alpha;
} lcd_pixel_t;


class LCD
{
   public:
      enum class color_depths
      {
         COLOR_DEPTH_8,
         COLOR_DEPTH_16,
         COLOR_DEPTH_24
      };
      LCD(uint32_t horizontal_res, uint32_t vertical_res, color_depths color_depth);
      void init(uint32_t frame_buffer_address);
      void config_layer(LTDC_LayerCfgTypeDef *layer_cfg); // Will likely change
      void flush();
      uint32_t get_horizontal_res(void);
      uint32_t get_vertical_res(void);
      color_depths get_color_depth(void);
      void set_pixel(uint32_t x, uint32_t y, lcd_pixel_t color);
      void set_frame_buffer_addr(uint32_t frame_buffer_addr);

   private:
      LTDC_HandleTypeDef   ltdcInstance;
      uint32_t             horizontal_res;
      uint32_t             vertical_res;
      color_depths         color_depth;
      uint32_t             frame_buffer_address;
      LTDC_LayerCfgTypeDef layer_cfg;
};

#endif