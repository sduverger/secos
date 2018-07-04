/* GPLv2 (c) Airbus */
#include <start.h>
#include <debug.h>
#include <pic.h>
#include <uart.h>
#include <intr.h>
#include <info.h>

uint32_t __mbh__ mbh_magic = MBH_MAGIC;
uint32_t __mbh__ mbh_flags = MBH_FLAGS;
sint32_t __mbh__ mbh_chksm = -(MBH_MAGIC+MBH_FLAGS);

static info_t __info __attribute__ ((aligned(16)));
       info_t *info = &__info;

void __attribute__((regparm(1))) start(mbi_t *mbi)
{
   info->mbi = mbi;

   pic_init();
   uart_init();
   intr_init();
   debug("\n" RELEASE " (c) Airbus\n");

   tp();

   panic("halted !");
}
