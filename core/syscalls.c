// Std lib stubs and implementations

#include "syscalls.h"
#include <stdio.h>

int _write(int file, char *ptr, int len)
{
   int DataIdx;

   for(DataIdx = 0; DataIdx < len; DataIdx++)
   {
      PUTCHAR(*ptr++);
   }

   return len;
}

// Dummy. May need to implement
caddr_t _sbrk(int incr)
{
   (void)incr;

   caddr_t ret = 0;

   return ret;
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

int _link(char* old, char* new) {

  return -1;
}

int _fork(void) {

  return -1;
}

int _execve(char* name, char** argv, char** env) {

  return -1;
}
