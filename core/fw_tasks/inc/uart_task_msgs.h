#ifndef __UART_TASK_MSGS_H
#define __UART_TASK_MSGS_H

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdint.h>

#include "fw_tasks.h"

constexpr uint32_t MAX_PRINT_SIZE = 32;

enum class UartOpcode
{
   UNDEFINED,
   XMODEM_RECEIVE,
   PRINT,
};

enum class XmodemStatus
{
   FAILED,
   DONE
};

/* Request Messages */
typedef struct _uart_xmodem_req_t
{
   generic_req_hdr_t  hdr;
   uint8_t           *buffer_ptr; // Pointer to buffer to place the requested data
   uint32_t           buffer_size;
} uart_xmodem_req_t;

typedef struct _uart_print_req_t
{
   generic_req_hdr_t hdr;
   uint32_t          msg_size;
   uint8_t           output[MAX_PRINT_SIZE]; // This can be bigger
} uart_print_req_t;

// Union of all messages
typedef union _uart_req_msg_u
{
   generic_req_msg_t generic_msg;
   uart_xmodem_req_t xmodem_req;
   uart_print_req_t  print_req;
} uart_req_msg_u;

/* Response Messages */
typedef struct _uart_xmodem_resp_t
{
   generic_resp_hdr_t hdr;
   XmodemStatus       status;
} uart_xmodem_resp_t;

typedef struct _uart_resp_msg_u
{
   generic_resp_msg_t generic_msg;
   uart_xmodem_resp_t xmodem_resp;
} uart_resp_msg_u;

#ifdef __cplusplus
}
#endif
#endif