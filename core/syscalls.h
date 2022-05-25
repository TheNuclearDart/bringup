// Maybe a bad filename
#ifndef __SYSCALLS_H
#define __SYSCALLS_H

#ifdef __cplusplus
extern "C" {
#endif

// These need to live in a separate file, not sure where yet, maybe something with print.cpp
// Also not sure how to use them with a single instance of Print
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define PUTCHAR(ch) __io_putchar(ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define PUTCHAR(ch, *f) fputc(ch, *f)
#endif

// Actual declaration. This is all messy.
PUTCHAR_PROTOTYPE;

#ifdef __cplusplus
}
#endif
#endif