/* GPLv2 (c) Airbus */
#ifndef __START_H__
#define __START_H__

#include <types.h>
#include <mbi.h>

extern void tp();

void __attribute__((regparm(1))) start(mbi_t*);

#endif
