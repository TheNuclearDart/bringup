/* LICENSE */

// Implementing user implemented FreeRTOS functions

#include "FreeRTOS.h"
#include "task.h"
#include "assert.h"

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName)
{
   (void)pxTask;
   (void)pcTaskName;

   assert_param(0); // Force assert.
}