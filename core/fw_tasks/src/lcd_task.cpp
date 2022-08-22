/* License */
#include "stm32f7xx.h"
#include "fw_tasks.h"
#include "lcd.h"
#include "memory_layout.h"
#include "print.h"
#include "queue.h"

// For messaging
#include "lcd_task_msgs.h"

namespace lcd_queues
{
   QueueHandle_t req;
   QueueHandle_t resp;
}

namespace
{
   LCD lcd;
   Print lcd_print("lcd");
}

void lcd_task_init(void)
{
   using namespace lcd_queues;

   req  = xQueueCreate(1, MAX_MSG_SIZE);
   resp = xQueueCreate(1, MAX_MSG_SIZE);

   lcd.init();

   LTDC_LayerCfgTypeDef layer_cfg = 
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
      .FBStartAdress = reinterpret_cast<uint32_t>(&_sfbuffer),
      .ImageWidth = 480,
      .ImageHeight = 272,
      .Backcolor =
      {
         .Blue = 0,
         .Green = 0,
         .Red = 0
      }
   };

   lcd.config_layer(&layer_cfg);

   lcd_print.init(&uart_queues::req);
}

void lcd_task(void *task_params)
{
   using namespace lcd_queues;

   lcd_print.out("Starting lcd task loop.\r\n");

   while(1)
   {
      lcd_print.handle_queue();
   }
}