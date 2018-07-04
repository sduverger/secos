/* GPLv2 (c) Airbus */
#ifndef __MATH_H__
#define __MATH_H__

#include <types.h>

static inline uint32_t abs(int x)
{
   return (x >= 0) ? x : -x;
}

/* precondition:  a > b */
static inline uint32_t pgcd(uint32_t a, uint32_t b)
{
   return b ? pgcd(b, a%b) : a;
}

#define max(a,b)  ((a)>(b)?(a):(b))
#define min(a,b)  ((a)<(b)?(a):(b))

#define __divrm(a,b,r,m)                                                \
   asm volatile ("div %%ecx":"=a"(r),"=d"(m):"a"(a),"d"(0),"c"(b))

#define adc16(a,b)                                                      \
   ({                                                                   \
      uint16_t v;                                                       \
      asm volatile (                                                    \
         "add %%dx, %%ax\n"                                             \
         "adc $0, %%ax"                                                 \
         :"=a"(v)                                                       \
         :"a"((a)),"d"((b))                                             \
         :"memory");                                                    \
      v;                                                                \
   })

#endif
