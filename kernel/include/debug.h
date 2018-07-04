/* GPLv2 (c) Airbus */
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <types.h>
#include <print.h>

#define debug(format, ...) printf(format, ## __VA_ARGS__)
void stack_trace(offset_t);

#endif
