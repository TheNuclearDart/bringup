#include <stdarg.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"

#include "uart_task_msgs.h"

constexpr uint8_t PRINT_QUEUE_SIZE = 8;

typedef struct _print_queue_t
{
   uart_print_req_t queued_reqs[PRINT_QUEUE_SIZE];
   uint8_t          head_idx = 0;
   uint8_t          tail_idx = 0;
} print_queue_t;

class Print
{
   public:
      int out(const char *format, ...);
      int vout(const char *format, va_list args);
      void init(QueueHandle_t *out_queue);
      void handle_queue(void);
      Print(const char *task_name);
   private:
      uint8_t        print_buffer[MAX_PRINT_SIZE];
      uint32_t       print_buffer_bytes;
      bool           print_in_progress;
      print_queue_t  print_queue;
      char           task_name[configMAX_TASK_NAME_LEN];
      QueueHandle_t *out_queue;
      void print_accumulate(const uint8_t &data);
      void puts(const char *str);
      void populate_buffer_header(void);
      void add_to_queue(const uart_print_req_t &print_req);
};