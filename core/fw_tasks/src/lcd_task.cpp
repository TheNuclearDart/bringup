/* License */
#include "stm32f7xx.h"
#include "fw_tasks.h"
#include "lcd.h"
#include "lvgl.h"
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
   LCD                lcd(480, 272, LCD::color_depths::COLOR_DEPTH_16);
   Print              lcd_print("lcd");
   lv_disp_draw_buf_t draw_buf;
   lv_color_t         buf1[(480 * 272) / 10];
   lv_color_t         buf2[(480 * 272) / 10]; // There might be a better solution to all of this
   lv_disp_drv_t      disp_drv;
   lv_obj_t          *btn;
   lv_disp_t         *disp;
}

// Basic implementation to start I guess
void lcd_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
   // There must be a much quicker way of doing this.
   // Also need to look into DMA for this.
   // But that also means using the HAL_LTDC library correctly, which I don't seem to be
   // As this writes directly to the framebuffer (you can physically see the pixels drawing)
   for (int32_t y = area->y1; y <= area->y2; y++)
   {
      for (int32_t x = area->x1; x <= area->x2; x++)
      {
         lcd.set_pixel(x, y, color_p->full);
         color_p++;
      }
   }
   //lcd.flush(); // This is what is actually causing the hard fault.
                  // It doesn't seem necessary to draw the screen, but why is
                  // it causing issues? (And what is it's purpose anyway?)
                  // Update: It appears to enable what is probably an unhandled interrupt.
                  // I think I'm using HAL_LTDC very wrong, so I need to try to understand
                  // how to actually use it.
                  // As it stands, this lcd_flush function draws to the screen, but _VERY SLOWLY_.
   lv_disp_flush_ready(disp);
}

void lcd_task_init(void)
{
   using namespace lcd_queues;

   req  = xQueueCreate(1, MAX_MSG_SIZE);
   resp = xQueueCreate(1, MAX_MSG_SIZE);

   lcd.init(reinterpret_cast<uint32_t>(&_sfbuffer));
   
   lv_init();

   lcd_print.init(&uart_queues::req);
}

// Temporary
void setup_screen(lv_obj_t *btn, uint32_t x, uint32_t y)
{
    //lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, x, y);                          /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    //lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_center(label);
}

void move_button(lv_obj_t *btn, uint32_t x, uint32_t y)
{
   lv_obj_set_pos(btn, x, y);
}

void lcd_task(void *task_params)
{
   using namespace lcd_queues;

   lv_disp_draw_buf_init(&draw_buf, buf1, buf2, (480 * 272) / 10);

   lv_disp_drv_init(&disp_drv);
   disp_drv.flush_cb = &lcd_flush;
   disp_drv.draw_buf = &draw_buf;
   disp_drv.hor_res  = lcd.get_horizontal_res();
   disp_drv.ver_res  = lcd.get_vertical_res();
   disp = lv_disp_drv_register(&disp_drv);

   lcd_print.out("Starting lcd task loop.\r\n");

   btn = lv_btn_create(lv_scr_act());
   if (btn == nullptr)
   {
      lcd_print.out("Null button pointer!!\r\n");
      HAL_Delay(1000);
      assert_param(0);
   }
   volatile uint32_t x = 180;
   volatile uint32_t y = 111;
   setup_screen(btn, x, y);

   while(1)
   {
      vTaskDelay(1); // Seems necessary to keep this task from blocking the others? For some reason?
      lcd_print.handle_queue();
      lv_task_handler();
   }
}