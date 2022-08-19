#ifndef __LCD_H
#define __LCD_H

#include "stm32f7xx.h"

class LCD
{
   public:
      LCD(void);
      void init(void);
      void config_layer(LTDC_LayerCfgTypeDef *layer_cfg); // Will likely change

   private:
      LTDC_HandleTypeDef ltdcInstance;
};

#endif