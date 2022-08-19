#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "print.h"

#include "uart_task_msgs.h"

namespace
{
   bool overall_print_in_progress = false;
}

/**
 * @brief Writes a string to the print buffer.
 *
 * This function takes a null-terminated string and iterates over each character,
 * accumulating them into the print buffer using the `print_accumulate` method.
 * The iteration continues until the end of the string is reached.
 *
 * @param str Pointer to the null-terminated string to be printed.
 */
void Print::puts(const char *str)
{
   while (*str != '\0')
   {
      this->print_accumulate(*str);
      str++;
   }
}

/**
 * @brief Adds a print request to the queue.
 *
 * This function enqueues a UART print request into the print queue. It checks if the queue is full
 * before adding the request. If the queue is not full, it copies the request into the queue and
 * updates the head index. If the queue is full, it triggers an assert.
 *
 * @param print_req The UART print request to be added to the queue.
 */
void Print::add_to_queue(const uart_print_req_t &print_req)
{
   if (this->print_queue.head_idx == (this->print_queue.tail_idx - 1))
   {
      // Full queue, assert for now
      assert_param(0);
   }
   memcpy(&this->print_queue.queued_reqs[this->print_queue.head_idx], &print_req, sizeof(print_req));
   if (this->print_queue.head_idx < (PRINT_QUEUE_SIZE - 1))
   {
      this->print_queue.head_idx++;
   }
   else
   {
      this->print_queue.head_idx = 0;
   }
}

/**
 * @brief Manages the processing of the print queue.
 *
 * This function checks if the print queue is not empty and initiates the print process if not already in progress.
 * It sends queued print requests to the UART request queue and updates the queue's tail index accordingly.
 * Once all requests are processed, it resets the print progress indicators.
 */
void Print::handle_queue(void)
{
   // Check if the queue is empty
   if (!(this->print_queue.head_idx == this->print_queue.tail_idx))
   {
      if (!overall_print_in_progress)
      {
         this->print_in_progress   = true;
         overall_print_in_progress = true;
      }
      if (this->print_in_progress)
      {
         while (this->print_queue.head_idx != this->print_queue.tail_idx)
         {
            xQueueSend(uart_queues::req, &this->print_queue.queued_reqs[this->print_queue.tail_idx], UINT32_MAX);
            if (this->print_queue.tail_idx < (PRINT_QUEUE_SIZE - 1))
            {
               this->print_queue.tail_idx++;
            }
            else
            {
               this->print_queue.tail_idx = 0;
            }
         }
         this->print_in_progress   = false;
         overall_print_in_progress = false;
      }
   }
}

/**
 * @brief Accumulate bytes into buffer, once full, send a message to print it
 * 
 * @param data char to accumulate
 */
void Print::print_accumulate(const uint8_t &data)
{
   // If our buffer hits max print size, or we reach the end of a print, send a print request message
   if (this->print_buffer_bytes == MAX_PRINT_SIZE || data == '\0')
   {
      // Construct and send the print message
      uart_print_req_t print_req = {};
      memcpy(&print_req.output, &print_buffer, MAX_PRINT_SIZE);
      print_req.hdr.opcode = static_cast<uint32_t>(UartOpcode::PRINT);
      print_req.msg_size   = this->print_buffer_bytes;

      // Another instance isn't printing, so let this instance begin printing
      if (!overall_print_in_progress)
      {
         overall_print_in_progress = true;
         this->print_in_progress   = true;
      }

      if (overall_print_in_progress)
      {
         // This instance is the one printing, so send to UART
         if (this->print_in_progress)
         {
            xQueueSend(uart_queues::req, &print_req, UINT32_MAX);
            this->print_buffer_bytes = 0;
         }
         // This instance _isn't_ the one printing, so add to this instance's internal queue.
         // This keeps the message from getting mangled by multiple instances stepping on each other
         else
         {
            this->add_to_queue(print_req);
            this->print_buffer_bytes = 0;
         }
      }

      if (data == '\0' && this->print_in_progress)
      {
         this->print_in_progress   = false;
         overall_print_in_progress = false;
      }
   }

   // Otherwise, add it to the buffer
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

/**
 * @brief Outputs a formatted string to the print buffer.
 *
 * This function interprets a formatted string and the variable arguments provided to construct
 * and output the final string. It supports printing literal percentages, integers, strings,
 * and hexadecimal values both in uppercase (with 'l' modifier) and lowercase.
 * The function accumulates each character into the print buffer and returns the total length
 * of the formatted output string.
 *
 * @param format The format string that contains the text to be written to the print buffer.
 *               It can optionally contain embedded format specifiers that are replaced by the
 *               values specified in subsequent additional arguments and formatted as requested.
 * @param args   A value identifying a variable arguments list initialized with va_start.
 * @return       The total number of characters written to the print buffer, or -1 if an error occurs.
 */
int Print::vout(const char *format, va_list args)
{
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
            case 'l': // Probably not the best method
               {
                  format++;
                  switch(*format)
                  {
                     case 'x':
                        // Hexadecimal
                        integer = va_arg(args, int);
                        itoa(integer, str_buffer, 16);
                        puts(str_buffer);
                        length += strlen(str_buffer);
                        break;
                  }
               }
               break;
            case 'x':
               // Hexadecimal. Need to unify this with 'lx'
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
   // Null terminate the string. This is a little different than printf.
   // This is here so that print_accumulate() knows this is the end of the string.
   this->print_accumulate('\0'); 
   return length;
}

/**
 * @brief Outputs a formatted string to the print buffer.
 *
 * This function is a variadic function that formats and sends a string to the print buffer,
 * similar to printf. It starts by initializing a variable argument list, then calls `vout`
 * to perform the formatting, and finally ends the variable argument list.
 *
 * @param format The format string for the output.
 * @return The length of the outputted string.
 */
int Print::out(const char *format, ...)
{
   int length = 0;
   va_list args;
   va_start(args, format);
   length = this->vout(format, args);
   va_end(args);

   return length;
}

/**
 * @brief Constructs a new Print object.
 *
 * This constructor initializes a new Print object with a specified task name.
 * It copies the task name to the object's internal storage, sets the print buffer byte count to zero,
 * and populates the buffer header.
 *
 * @param task_name A pointer to a character array containing the task name to be assigned to this print object.
 */
Print::Print(const char *task_name)
{
   strcpy(this->task_name, task_name);

   this->print_buffer_bytes = 0;
   this->populate_buffer_header();
}

/**
 * @brief Initializes the Print object with an output queue.
 *
 * This function assigns the provided queue to the Print object's internal queue handler.
 * It should be called to set up the Print object's output queue before starting print operations.
 *
 * @param out_queue A pointer to the queue handle that will be used for print operations.
 */
void Print::init(QueueHandle_t *out_queue)
{
   this->out_queue = out_queue;
}

/**
 * @brief Populates the header of the print buffer with the task name.
 *
 * This function constructs a header for the print buffer by enclosing the task name within square brackets.
 * It then adds a space character after the closing bracket to separate the header from subsequent content.
 * The header format is: [task_name] 
 */
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