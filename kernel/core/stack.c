/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern offset_t __kernel_start__;

void stack_trace(offset_t from)
{
   offset_t *ebp, *eip;

   printf("\n-= Stack Trace =-\n");

   ebp = (offset_t*)from;
   while(ebp && ebp < &__kernel_start__)
   {
      eip = (offset_t*)(*(ebp+1));
      ebp = (offset_t*)(*ebp);
      printf("%p\n", (offset_t)eip);
   }
}
