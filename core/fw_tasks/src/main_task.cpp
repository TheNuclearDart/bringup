#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "assert.h"
#include "crc.h"
#include "fw_image.h"
#include "fw_tasks.h"
#include "fw_update.h"
#include "lcd.h"
#include "print.h"
#include "queue.h"
#include "syscalls.h"
#include "uart.h"

// For messaging
#include "main_task_msgs.h"
#include "uart_task_msgs.h"

namespace
{
   // Need to decouple this more from the HAL
   // moving to UART taskUART uart(USART1, 115200, UART_WORDLENGTH_8B, UART_STOPBITS_1, UART_PARITY_NONE, UART_MODE_TX_RX, UART_HWCONTROL_NONE, UART_OVERSAMPLING_16, UART_ONE_BIT_SAMPLE_DISABLE, 1000);
   LCD lcd;
   uint8_t fw_image_buffer[0x10000]; // Arbitrary size, but it's going to need to be bigger (and likely in external RAM) once the image gets bigger
   generic_task_ctx_t uart_ctx;
   Print main_print("main");
   //USB_Host usb;
}

QueueHandle_t main_req_queue;
QueueHandle_t main_resp_queue;

// Not sure what to do with this now... not using printf, but _write stil needs it.
// There's probably a way to stop linking printf
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

// This dead right now.
PUTCHAR_PROTOTYPE
{
   //uart.out(ch, 1);
   return ch;
}

int print_wrapper(const char *format, ...)
{
   int length = 0;
   va_list args;
   va_start(args, format);
   length = main_print.vout(format, args);
   va_end(args);

   return length;
}

void main_task_init(void)
{
   // Create Queues to other tasks
   main_req_queue  = xQueueCreate(1, MAX_MSG_SIZE); // One message queue (for now), size of 128 bytes. These should be defined.
   main_resp_queue = xQueueCreate(1, MAX_MSG_SIZE);
   
   //uart.init();
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

   crc_init();
   fw_image_init(&crc32_calculate, &print_wrapper); // I feel like I need a different solution for this.

   main_print.init(&uart_req_queue);
}

/* Message processing will likely be moved into its own file someday */
/**
 * @brief Handles an input notification message
 * 
 * @param input_notification Input notification msg reference
 */
void handle_input_notification(input_notification_msg_t &input_notification)
{
   // Check the input and take action
   switch(input_notification.input)
   {
      case InputType::BLUE_BUTTON:
         {
            // Construct and send an Xmodem receive req
            uart_ctx = {};
            uart_ctx.opcode = static_cast<uint32_t>(MainOpcode::XMODEM_RECEIVE);

            uart_xmodem_req_t xmodem_req = {};
            xmodem_req.buffer_ptr     = fw_image_buffer;
            xmodem_req.buffer_size    = 0x10000;
            xmodem_req.hdr.opcode     = static_cast<uint32_t>(UartOpcode::XMODEM_RECEIVE);
            xmodem_req.hdr.ctx        = &uart_ctx;
            xmodem_req.hdr.resp_queue = &main_resp_queue;

            xQueueSend(uart_req_queue, &xmodem_req, UINT32_MAX);
         }
         break;
      case InputType::UNDEFINED:
      default:
         assert_param(0);
   }
}

/**
 * @brief Handle a request message for this task
 * 
 * @param req_msg Reference to the message
 */
void handle_request(main_req_msg_u &req_msg)
{
   MainOpcode opcode = static_cast<MainOpcode>(req_msg.generic_msg.generic_hdr.opcode);

   switch (opcode)
   {
      case MainOpcode::INPUT_NOTIFICATION:
         handle_input_notification(req_msg.input_notification);
         break;
      case MainOpcode::UNDEFINED:
      default:
         assert_param(0);
   }
}

void handle_resp(generic_resp_msg_t &resp_msg)
{
   // Determine how to treat this based off of the attached context
   MainOpcode opcode = static_cast<MainOpcode>(resp_msg.generic_hdr.ctx->opcode);

   switch(opcode)
   {
      case MainOpcode::XMODEM_RECEIVE:
         {
            uart_xmodem_resp_t xmodem_resp = reinterpret_cast<uart_xmodem_resp_t &>(resp_msg);
            switch (xmodem_resp.status)
            {
               case XmodemStatus::DONE:
                  {
                     fw_update_error_e update_error = fw_update(fw_image_buffer);
                     if (update_error == fw_update_error_e::SUCCESS)
                     {
                        main_print.out("Update complete, reset device to activate new image.\r\n");
                        //NVIC_SystemReset();
                     }
                  }
                  break;
               case XmodemStatus::FAILED:
                  // Need to handle this instead of asserting
               default:
                  assert_param(0);
            }
            break;
         }
      case MainOpcode::INPUT_NOTIFICATION:
      case MainOpcode::UNDEFINED:
      default:
         assert_param(0);
   }
}

void main_task(void *task_params)
{
   main_print.out("Starting main task loop.\r\n");

   while(1)
   {
      main_req_msg_u     req_msg  = {};
      generic_resp_msg_t resp_msg = {};

      main_print.handle_queue();

      if (xQueueReceive(main_resp_queue, &resp_msg, 0) == pdTRUE)
      {
         handle_resp(resp_msg);
      }
      if (xQueueReceive(main_req_queue, &req_msg, 0) == pdTRUE)
      {
         /* Handle req messages */
         handle_request(req_msg);
      }
   }
}