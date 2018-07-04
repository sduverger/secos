/* GPLv2 (c) Airbus */
#ifndef __PRINT_H__
#define __PRINT_H__

#include <types.h>

#define  va_start(v,l)           __builtin_va_start(v,l)
#define  va_end(v)               __builtin_va_end(v)
#define  va_arg(v,l)             __builtin_va_arg(v,l)
typedef  __builtin_va_list       va_list;

void     panic(const char*, ... );
size_t   printf(const char*, ... );
size_t   snprintf(char*, size_t, const char*, ...);

size_t   __vprintf(const char*, va_list);
size_t   __vsnprintf(char*, size_t, const char*, va_list);

#endif
