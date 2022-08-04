#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"

#include "uart_task_msgs.h"
class Print
{
   public:
      int out(const char *format, ...);
      void init(QueueHandle_t *out_queue);
      Print(const char *task_name);
   private:
      uint8_t  print_buffer[MAX_PRINT_SIZE];
      uint32_t print_buffer_bytes;
      char     task_name[configMAX_TASK_NAME_LEN];
      QueueHandle_t *out_queue;
      void print_accumulate(const uint8_t &data);
      void puts(const char *str);
      void populate_buffer_header(void);
};