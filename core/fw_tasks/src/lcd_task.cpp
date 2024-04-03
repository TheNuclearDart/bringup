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
   LCD                     lcd(480, 272, LCD::color_depths::COLOR_DEPTH_24);
   Print                   lcd_print("lcd");
   FRAME_BUFFER lv_color_t buf1[480 * 272];
   FRAME_BUFFER lv_color_t buf2[480 * 272]; // LVGL Direct mode requires screen sized buffers, but not sure if we even want to use that mode
   lv_obj_t                *btn;
   lv_disp_t               *disp;
}

// Basic implementation to start I guess
void lcd_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
   // There must be a much quicker way of doing this.
   // Also need to look into DMA for this.
   // But that also means using the HAL_LTDC library correctly, which I don't seem to be
   // As this writes directly to the framebuffer (you can physically see the pixels drawing)
   /*for (int32_t y = area->y1; y <= area->y2; y++)
   {
      for (int32_t x = area->x1; x <= area->x2; x++)
      {
         lcd_pixel_t *color = reinterpret_cast<lcd_pixel_t *>(px_map);
         lcd.set_pixel(x, y, *color);
         color++;
      }
   }*/

   // 4/2024 According to LVGL, should just swap buffers when using full refresh. "This means in flush_cb, only the address of the frame buffer needs to be changed to the provided pointer (color_p @JDM I think this is now px_map)"
   lcd.set_frame_buffer_addr(reinterpret_cast<uint32_t>(px_map));
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

   disp = lv_display_create(480, 272);

   lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_DIRECT); // Direct mode requires screen sized buffers, but what mode should we even be using?

   lv_display_set_flush_cb(disp, lcd_flush);

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

   lcd_print.out("Starting lcd task loop.\r\n");

   while(1)
   {
      vTaskDelay(1); // Seems necessary to keep this task from blocking the others? For some reason?
      lcd_print.handle_queue();
      lv_task_handler();
   }
}