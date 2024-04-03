#include "stm32f7xx.h"
#include "fw_tasks.h"
#include "gpio_defines.h"
#include "print.h"
#include "queue.h"

// For messaging
#include "main_task_msgs.h"

namespace input_queues
{
   QueueHandle_t req;
   QueueHandle_t resp;
}

namespace
{
   Print input_print("input");
}

void input_task_init(void)
{
   using namespace input_queues;

   // Create Queues to other tasks
   req  = xQueueCreate(1, MAX_MSG_SIZE); // One message queue (for now), size of 128 bytes. These should be defined.
   resp = xQueueCreate(1, MAX_MSG_SIZE);

   input_print.init(&uart_queues::req);
}

static void handle_input(void)
{
   input_notification_msg_t input_notification = {};

   input_notification.hdr.opcode     = static_cast<uint32_t>(MainOpcode::INPUT_NOTIFICATION);
   input_notification.hdr.resp_queue = nullptr;
   input_notification.input          = InputType::BLUE_BUTTON;

   xQueueSend(main_queues::req, &input_notification, 0); // Returns if successful, I don't think we need to bother with that for now.
}

void input_task(void *task_params) // Does this *have* to be void * for freeRTOS?
{
   input_print.out("Starting input task loop.\r\n");
   while (1)
   {
      input_print.handle_queue();
      // This needs some work to prevent it from sending endless messages??
      GPIO_PinState read_val = HAL_GPIO_ReadPin(USER_PUSH_BUTTON_PORT, USER_PUSH_BUTTON_PIN);
      if (read_val == GPIO_PIN_SET)
      {
         handle_input();
         // Basic delay to debounce. Need a better option.
         HAL_Delay(500);
      }
      //if (xQueueReceive(resp, &msg, 0) == pdTRUE)
      //{
         /* Handle msg */
      //}
      //if (xQueueReceive(req, &msg, 0) == pdTRUE)
      //{
      //   /* Handle req messages */
      //}
   }
}