/* GPLv2 (c) Airbus */
#ifndef __ASM_H__
#define __ASM_H__

#include <gpr.h>

#define force_interrupts_on()     asm volatile( "sti ; nop" )
#define force_interrupts_off()    asm volatile( "cli" )
#define halt()                    asm volatile( "cli ; hlt" )

#define interrupts_enabled()         (get_flags() & EFLAGS_IF)
#define disable_interrupts(flags)    ({save_flags(flags);force_interrupts_on();})
#define enable_interrupts(flags)     ({save_flags(flags);force_interrupts_off();})
#define restore_interrupts(flags)    load_flags(flags)

#endif
