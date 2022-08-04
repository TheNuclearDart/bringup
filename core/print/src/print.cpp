#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "print.h"

#include "uart_task_msgs.h"

namespace
{
}

void Print::puts(const char *str)
{
   while (*str != '\0')
   {
      this->print_accumulate(*str);
   }
}
/**
 * @brief Accumulate bytes into buffer, once full, send a message to print it
 * 
 * @param data char to accumulate
 */
void Print::print_accumulate(const uint8_t &data)
{
   if (this->print_buffer_bytes == MAX_PRINT_SIZE || data == '\0')
   {
      // Construct and send the print message
      uart_print_req_t print_req = {};
      memcpy(&print_req.output, &print_buffer, MAX_PRINT_SIZE);
      print_req.hdr.opcode = static_cast<uint32_t>(UartOpcode::PRINT);
      print_req.msg_size = this->print_buffer_bytes;

      xQueueSend(uart_req_queue, &print_req, UINT32_MAX);
      this->print_buffer_bytes = 0;
   }

   if (data != '\0')
   {
      this->print_buffer[print_buffer_bytes] = data;
      this->print_buffer_bytes++;
   }
   else
   {
      // Repopulate the buffer with the task name
      this->populate_buffer_header();
   }
}

int Print::out(const char *format, ...)
{
   va_list args;
   va_start(args, format);

   int length = 0;

   int integer;
   char *str;

   char str_buffer[32]; // How large should this actually be?

   while(*format != '\0')
   {
      if (*format == '%')
      {
         // We've hit a %, so check the next char to know what to do
         format++;
         switch(*format)
         {
            case '%':
               // Single %
               this->print_accumulate(*format);
               length++;
               break;
            case 'd':
               // Integer
               integer = va_arg(args, int);
               itoa(integer, str_buffer, 10);
               puts(str_buffer);
               length += strlen(str_buffer);
               break;
            case 's':
               // String
               str = va_arg(args, char *);
               puts(str);
               length += strlen(str);
               break;
            case 'x':
               // Hexadecimal
               integer = va_arg(args, int);
               itoa(integer, str_buffer, 16);
               puts(str_buffer);
               length += strlen(str_buffer);
               break;
            default:
               return -1;
         }
      }
      else
      {
         this->print_accumulate(*format);
         length++;
      }
      format++;
   }
   this->print_accumulate('\0'); // Null terminate the string. This is a little different than printf.
   return length;
}

Print::Print(const char *task_name)
{
   strcpy(this->task_name, task_name);

   this->print_buffer_bytes = 0;
   this->populate_buffer_header();
}

void Print::init(QueueHandle_t *out_queue)
{
   this->out_queue = out_queue;
}

void Print::populate_buffer_header(void)
{
   this->print_buffer[print_buffer_bytes] = '[';
   this->print_buffer_bytes++;
   memcpy(&this->print_buffer[print_buffer_bytes], this->task_name, strlen(task_name));
   this->print_buffer_bytes += strlen(task_name);
   this->print_buffer[print_buffer_bytes] = ']';
   this->print_buffer_bytes++;
   this->print_buffer[print_buffer_bytes] = ' ';
   this->print_buffer_bytes++;
}