/* License */
#include "stm32f7xx.h"
#include "fw_tasks.h"
#include "gpio_defines.h"
#include "print.h"
#include "queue.h"

// For messaging
#include "lcd_task_msgs.h"

namespace lcd_queues
{
   QueueHandle_t req;
   QueueHandle_t resp;
}