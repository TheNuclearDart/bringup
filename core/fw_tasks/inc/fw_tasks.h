#ifndef __FW_TASKS_H
#define __FW_TASKS_H

#ifdef __cplusplus
    extern "C" {
#endif

#include "FreeRTOS.h"
#include "queue.h"

#define MAX_MSG_SIZE 128

// Generic task context
typedef struct _generic_task_ctx_t
{
   uint32_t opcode;
} generic_task_ctx_t;

// Generic message types
typedef struct _generic_req_hdr_t
{
   uint32_t            opcode;
   generic_task_ctx_t *ctx;
   QueueHandle_t      *resp_queue;
} generic_req_hdr_t;

typedef struct _generic_resp_msg_hdr_t
{
   generic_task_ctx_t *ctx;
} generic_resp_hdr_t;

typedef struct _generic_req_msg_t
{
   generic_req_hdr_t generic_hdr;
   uint8_t           rsvd[MAX_MSG_SIZE - sizeof(generic_hdr)];
} generic_req_msg_t;
static_assert(sizeof(generic_req_msg_t) == MAX_MSG_SIZE); // Enforce a message size of 128 bytes

typedef struct _generic_resp_msg_t
{
   generic_resp_hdr_t generic_hdr;
   uint8_t            rsvd[MAX_MSG_SIZE - sizeof(generic_hdr)];
} generic_resp_msg_t;
static_assert(sizeof(generic_resp_msg_t) == MAX_MSG_SIZE);

// Task Init functions
void main_task_init(void);
void input_task_init(void);
void uart_task_init(void);

// Task main functions
void main_task(void *task_params);
void input_task(void *task_params);
void uart_task(void *task_params);

/* Task Queues, declared here for visibility, created in owning task */
// Main Task
extern QueueHandle_t main_req_queue;
extern QueueHandle_t main_resp_queue;

// Input Task
extern QueueHandle_t input_req_queue;
extern QueueHandle_t input_resp_queue;

// UART Task
extern QueueHandle_t uart_req_queue;
extern QueueHandle_t uart_resp_queue;

#ifdef __cplusplus
}
#endif
#endif