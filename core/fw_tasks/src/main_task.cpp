#include <stdio.h>

#include "assert.h"
#include "crc.h"
#include "fw_image.h"
#include "fw_tasks.h"
#include "lcd.h"
#include "main_task_msgs.h"
#include "queue.h"
#include "syscalls.h"
#include "uart.h"

namespace
{
   // Need to decouple this more from the HAL
   UART uart(USART1, 115200, UART_WORDLENGTH_8B, UART_STOPBITS_1, UART_PARITY_NONE, UART_MODE_TX_RX, UART_HWCONTROL_NONE, UART_OVERSAMPLING_16, UART_ONE_BIT_SAMPLE_DISABLE, 1000);
   LCD lcd;
   uint8_t fw_image_buffer[0x10000]; // Arbitrary size, but it's going to need to be bigger (and likely in external RAM) once the image gets bigger
   //USB_Host usb;
}

QueueHandle_t main_req_queue;
QueueHandle_t main_resp_queue;

// These need to live in a separate file, not sure where yet, maybe something with print.cpp
// Also not sure how to use them with a single instance of Print
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
   uart.out(ch, 1);
   return ch;
}

void main_task_init(void)
{
   // Create Queues to other tasks
   main_req_queue  = xQueueCreate(1, MAX_MSG_SIZE); // One message queue (for now), size of 128 bytes. These should be defined.
   main_resp_queue = xQueueCreate(1, MAX_MSG_SIZE);
   
   uart.init();
   lcd.init();

   crc_init();
   fw_image_init(&crc32_calculate); // I feel like I need a different solution for this.
}

/* Message processing will likely be moved into its own file someday */
/**
 * @brief Handles an input notification message
 * 
 * @param input_notification Input notification msg reference
 */
void handle_input_notification(input_notification_msg_t &input_notification)
{
   printf("Received an input!\r\n");
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
         assert_msg(0, "Unhandled Message!!\r\n");
   }
}

void main_task(void *task_params)
{
   while(1)
   {
      main_req_msg_u  req_msg  = {};
      main_resp_msg_u resp_msg = {};
      if (xQueueReceive(main_resp_queue, &resp_msg, 0) == pdTRUE)
      {
         /* Handle resp messages */
      }
      if (xQueueReceive(main_req_queue, &req_msg, 0) == pdTRUE)
      {
         /* Handle req messages */
         handle_request(req_msg);
      }
   }
}