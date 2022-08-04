#ifndef __MAIN_TASK_MSGS_H
#define __MAIN_TASK_MSGS_H

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdint.h>

#include "fw_tasks.h"

enum class InputType
{
   UNDEFINED,
   BLUE_BUTTON
};

enum class MainOpcode
{
   UNDEFINED,
   INPUT_NOTIFICATION,
   XMODEM_RECEIVE,
};

/* Request Messages */
typedef struct _input_notification_msg_t
{
   generic_req_hdr_t hdr;
   InputType input = InputType::UNDEFINED;
} input_notification_msg_t;

// Union of all messages
typedef union _main_req_msg_u
{
   generic_req_msg_t        generic_msg;
   input_notification_msg_t input_notification;
} main_req_msg_u;

/* Response Messages */
typedef struct _main_resp_msg_u
{
   generic_resp_msg_t generic_msg;
} main_resp_msg_u;

#ifdef __cplusplus
}
#endif
#endif