#ifndef __LCD_H
#define __LCD_H

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

class LCD
{
   public:
      LCD(void);
      void init(void);

   private:
      LTDC_HandleTypeDef ltdcInstance;
};

#endif