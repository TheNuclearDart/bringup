#include "stm32f7xx.h"
#include "fw_tasks.h"
#include "gpio_defines.h"
#include "print.h"
#include "queue.h"
#include "uart.h"

// For messaging
#include "uart_task_msgs.h"

namespace
{
   UART uart(USART1, 115200, UART_WORDLENGTH_8B, UART_STOPBITS_1, UART_PARITY_NONE, UART_MODE_TX_RX, UART_HWCONTROL_NONE, UART_OVERSAMPLING_16, UART_ONE_BIT_SAMPLE_DISABLE, 1000);
}

QueueHandle_t uart_req_queue;
QueueHandle_t uart_resp_queue;

void uart_task_init(void)
{
   // Create Queues to other tasks
   uart_req_queue  = xQueueCreate(1, MAX_MSG_SIZE); // One message queue (for now), size of 128 bytes. These should be defined.
   uart_resp_queue = xQueueCreate(1, MAX_MSG_SIZE);

   uart.init();
}

static XmodemStatus handle_xmodem_req(uart_xmodem_req_t &xmodem_req)
{
   //taskENTER_CRITICAL();
   vTaskSuspendAll();
   UART::error error = uart.xmodem_receive(xmodem_req.buffer_ptr, xmodem_req.buffer_size);
   xTaskResumeAll();
   //taskEXIT_CRITICAL();

   if (error == UART::error::OKAY)
   {
      return XmodemStatus::DONE;
   }

   return XmodemStatus::FAILED;
}

static void handle_print_req(uart_print_req_t &print_req)
{
   for (uint32_t i = 0; i < print_req.msg_size; i++)
   {
      uart.out(print_req.output[i], 1);
   }
}

static void handle_uart_req(uart_req_msg_u &req_msg)
{
   UartOpcode opcode = static_cast<UartOpcode>(req_msg.generic_msg.generic_hdr.opcode);

   switch(opcode)
   {
      case UartOpcode::XMODEM_RECEIVE:
         {
            // Start Xmodem receive process.
            // This would be blocking (I think all UART work would be)
            XmodemStatus status = handle_xmodem_req(req_msg.xmodem_req);
            // Construct the response message
            uart_xmodem_resp_t xmodem_resp = {};

            xmodem_resp.hdr.ctx = req_msg.generic_msg.generic_hdr.ctx; // Pass the generic ctx back to sender
            xmodem_resp.status  = status;
            xQueueSend(*req_msg.generic_msg.generic_hdr.resp_queue, &xmodem_resp, 0);
         }
         break;
      case UartOpcode::PRINT:
         // Handle print process
         handle_print_req(req_msg.print_req);
         break;
      case UartOpcode::UNDEFINED:
      default:
         assert_param(0);
   } 

}

void uart_task(void *task_params) // Does this *have* to be void * for freeRTOS?
{
   uart.out_raw((uint8_t *)"[uart] Starting UART task loop.\r\n", 33, UINT32_MAX); // Doing this as the print library doesn't currently work for this task.
   while(1)
   {
      uart_req_msg_u  req_msg  = {};
      uart_resp_msg_u resp_msg = {};
      if (xQueueReceive(uart_resp_queue, &resp_msg, 0) == pdTRUE)
      {
         /* Handle msg */
      }
      if (xQueueReceive(uart_req_queue, &req_msg, 0) == pdTRUE)
      {
         handle_uart_req(req_msg);
      }
   }
}