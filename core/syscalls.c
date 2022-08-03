// Std lib stubs and implementations

#include "memory_layout.h"
#include "syscalls.h"
#include <stdio.h>

#if !BOOTLOADER_BUILD
   #include "FreeRTOS.h"
   #include "task.h"
#endif

int _write(int file, char *ptr, int len)
{
   int DataIdx;

   for(DataIdx = 0; DataIdx < len; DataIdx++)
   {
      PUTCHAR(*ptr++);
   }

   return len;
}

// Absolutely needed for printf
// Maybe this is too simple for FreeRtos?? But it seems like we never reach here anyway.
caddr_t _sbrk(int incr)
{
   uint8_t *heap = &end; // This 'end' is the start of the heap, defined in .ld script

   uint8_t *prev_heap = heap;

   heap += incr;

   return (caddr_t)prev_heap;
}

int _close(int file) {
  return -1;
}

struct stat;
int _fstat(int file, struct stat* st) {

  return 0;
}

int _isatty(int file) {
  return 1;
}

int _lseek(int file, int ptr, int dir) {
  return 0;
}

int _read(int file, char* ptr, int len) {
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++) {
    // fix me*ptr++ = __io_getchar();
  }

  return len;
}

int _open(char* path, int flags, ...) {
  /* Pretend like we always fail */
  return -1;
}

int _wait(int* status) {

  return -1;
}

int _unlink(char* name) {

  return -1;
}

struct tms;
int _times(struct tms* buf) {
  return -1;
}

struct stat;
int _stat(char* file, struct stat* st) {

  return -1;
}

int _link(char* old, char* new_char) {

  return -1;
}

int _fork(void) {

  return -1;
}

int _execve(char* name, char** argv, char** env) {

  return -1;
}

#if !BOOTLOADER_BUILD
void __malloc_lock (struct _reent *reent)
{
   vTaskSuspendAll();
}

void __malloc_unlock(struct _reent *reent)
{
   xTaskResumeAll();
}
#endif