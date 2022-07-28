#include "stm32f7xx.h"
#include "fw_tasks.h"
#include "gpio_defines.h"
#include "queue.h"

// For messaging
#include "main_task_msgs.h"

namespace
{
}

QueueHandle_t input_req_queue;
QueueHandle_t input_resp_queue;

void input_task_init(void)
{
   // Create Queues to other tasks
   input_req_queue  = xQueueCreate(1, MAX_MSG_SIZE); // One message queue (for now), size of 128 bytes. These should be defined.
   input_resp_queue = xQueueCreate(1, MAX_MSG_SIZE);
}

static void handle_input(void)
{
   input_notification_msg_t input_notification = {};

   input_notification.hdr.opcode     = static_cast<uint32_t>(MainOpcode::INPUT_NOTIFICATION);
   input_notification.hdr.resp_queue = nullptr;
   input_notification.input          = InputType::BLUE_BUTTON;

   xQueueSend(main_req_queue, &input_notification, 0); // Returns if successful, I don't think we need to bother with that for now.
}

void input_task(void *task_params) // Does this *have* to be void * for freeRTOS?
{
   while(1)
   {
      GPIO_PinState read_val = HAL_GPIO_ReadPin(USER_PUSH_BUTTON_PORT, USER_PUSH_BUTTON_PIN);
      if (read_val == GPIO_PIN_SET)
      {
         handle_input();
      }
      //if (xQueueReceive(input_resp_queue, &msg, 0) == pdTRUE)
      //{
         /* Handle msg */
      //}
      //if (xQueueReceive(input_req_queue, &msg, 0) == pdTRUE)
      //{
      //   /* Handle req messages */
      //}
   }
}