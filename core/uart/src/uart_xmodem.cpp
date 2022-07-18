#include "uart.h"

namespace
{
   constexpr uint8_t  SOH         = 0x01; // Start of Header
   constexpr uint8_t  EOT         = 0x04; // End of Text
   constexpr uint8_t  ACK         = 0x06; // Acknowledge
   constexpr uint8_t  NAK         = 0x15; // Negative Ack
   constexpr uint8_t  CAN         = 0x18; // Cancel
   constexpr uint8_t  ESC         = 0x1b; // Escape
   constexpr uint32_t PACKET_SIZE = 128;
   constexpr uint16_t CRC16_POLY  = 0x1021;
}

/**
 * @brief Receive data over xmodem
 * 
 * @param buffer Pointer to buffer to hold received data
 * @param size Size of given buffer, to prevent overrun if received data is larger
 */
UART::error UART::xmodem_receive(uint8_t *buffer, const uint32_t size)
{
   uint8_t received_char;
   uint8_t timeout = 0;

   // Block and print 'C' until data is received. Timeout after 20 seconds
   while (1)
   {
      this->out('C', 1);

      uint32_t tickstart = HAL_GetTick();
      if (UART_WaitOnFlagUntilTimeout(&this->uartInstance, UART_FLAG_RXNE, RESET, tickstart, 1000) == HAL_OK)
      {
         break;
      } // Replace with something in the UART class

      if (this->receive_char(received_char) != error::TIMEOUT)
      {
         break;
      }
      if (timeout == 120)
      {
         return error::TIMEOUT;
      }
      timeout++;
   }

   bool done = false;
   uint32_t num_bytes = 0;
   error error = error::OKAY;
   uint8_t sequence_number = 1;
   while (!done)
   {
      this->receive_char(received_char);
      switch(received_char)
      {
         case SOH:
            // Check that we won't overrun given buffer?
            if ((num_bytes + PACKET_SIZE) > size)
            {
               done = true;
               error = error::BUFFER_OVERRUN;
            }
            error = this->xmodem_receive_packet(buffer+num_bytes, sequence_number);
            if (error == error::OKAY)
            {
               sequence_number++;
               num_bytes += PACKET_SIZE;
            }
            else
            {
               done = true;
            }
            
            this->out(ACK, 1);
            break;
         
         case EOT:
            this->out(ACK, 1);
            done = true;
            break;
         
         case CAN:
            // Intentional fallthrough
         case ESC:
            // Intentional fallthrough
         default:
            done = true;
            break;
      }
   }
   
   return error;
}

uint16_t UART::xmodem_receive_bytes(uint8_t *buffer, const uint32_t size)
{
   uint16_t crc = 0;
   uint8_t byte;

   for (uint32_t i = 0; i < size; ++i)
   {
      this->receive_char(byte);
      crc = crc ^ (uint32_t) byte << 8;
      for (uint32_t j = 0; j < 8; j++)
      {
         if (crc & 0x8000)
         {
            crc = crc << 1 ^ CRC16_POLY;
         }
         else
         {
            crc = crc << 1;
         }
      }
      crc = crc & 0xFFFF;
      *buffer++ = byte;
   }
   
   return crc;
}

UART::error UART::xmodem_receive_packet(uint8_t *buffer, const uint8_t sequence_num)
{
   // First byte (SOH) has already been received by calling function
   uint8_t temp[2];
   uint8_t packet_sequence_number[2];
   uint16_t crc;
   uint16_t xcrc;

   // When working on this again, start with implementing this function, as well as "xmodem_receive_bytes"
   // I think these are now implemented but needs testing`

   this->xmodem_receive_bytes((uint8_t *)packet_sequence_number, 2); // Does this need to be xmodem_receive, or just uart receive?

   xcrc = xmodem_receive_bytes(buffer, PACKET_SIZE);

   this->receive_char(temp[0]);
   this->receive_char(temp[1]);

   // Combine crc bytes into one 16 bit value
   crc = temp[0] << 8;
   crc += temp[1];

   if ((crc != xcrc) || (packet_sequence_number[0] != sequence_num) || (packet_sequence_number[1] != (255 - sequence_num)))
   {
      this->out(CAN, 1);
      return error::GEN_ERROR;
   }

   return error::OKAY;
}