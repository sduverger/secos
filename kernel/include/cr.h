/* GPLv2 (c) Airbus */
#ifndef __CR_H__
#define __CR_H__

#include <types.h>

/*
** CR0
*/
#define CR0_PE_BIT      0
#define CR0_MP_BIT      1
#define CR0_EM_BIT      2
#define CR0_TS_BIT      3
#define CR0_ET_BIT      4
#define CR0_NE_BIT      5
#define CR0_WP_BIT     16
#define CR0_AM_BIT     18
#define CR0_NW_BIT     29
#define CR0_CD_BIT     30
#define CR0_PG_BIT     31

#define CR0_PE         (1UL<<CR0_PE_BIT)
#define CR0_MP         (1UL<<CR0_MP_BIT)
#define CR0_EM         (1UL<<CR0_EM_BIT)
#define CR0_TS         (1UL<<CR0_TS_BIT)
#define CR0_ET         (1UL<<CR0_ET_BIT)
#define CR0_NE         (1UL<<CR0_NE_BIT)
#define CR0_WP         (1UL<<CR0_WP_BIT)
#define CR0_AM         (1UL<<CR0_AM_BIT)
#define CR0_NW         (1UL<<CR0_NW_BIT)
#define CR0_CD         (1UL<<CR0_CD_BIT)
#define CR0_PG         (1UL<<CR0_PG_BIT)

typedef union control_register_0
{
   struct
   {
      uint32_t    pe:1;   /* protected mode */
      uint32_t    mp:1;   /* monitor copro */
      uint32_t    em:1;   /* emulation */
      uint32_t    ts:1;   /* task switch */
      uint32_t    et:1;   /* ext type */
      uint32_t    ne:1;   /* num error */
      uint32_t    r1:10;  /* reserved */
      uint32_t    wp:1;   /* write protect */
      uint32_t    r2:1;   /* reserved */
      uint32_t    am:1;   /* align mask */
      uint32_t    r3:10;  /* reserved */
      uint32_t    nw:1;   /* not write through */
      uint32_t    cd:1;   /* cache disable */
      uint32_t    pg:1;   /* paging */

   } __attribute__((packed));

   raw32_t;

} __attribute__((packed)) cr0_reg_t;

/*
** CR3
*/
typedef union control_register_3
{
   struct
   {
      uint32_t  r1:3;
      uint32_t  pwt:1;
      uint32_t  pcd:1;
      uint32_t  r2:7;
      uint32_t  addr:20;

   } __attribute__((packed));

   raw32_t;

} __attribute__((packed)) cr3_reg_t;

/*
** CR4
*/
#define CR4_VME_BIT          0
#define CR4_PVI_BIT          1
#define CR4_TSD_BIT          2
#define CR4_DE_BIT           3
#define CR4_PSE_BIT          4
#define CR4_PAE_BIT          5
#define CR4_MCE_BIT          6
#define CR4_PGE_BIT          7
#define CR4_PCE_BIT          8
#define CR4_OSFXSR_BIT       9
#define CR4_OSXMMEXCPT_BIT  10
#define CR4_VMXE_BIT        13
#define CR4_SMXE_BIT        14
#define CR4_PCIDE_BIT       17
#define CR4_OSXSAVE_BIT     18
#define CR4_SMEP_BIT        20

#define CR4_VME             (1UL<<CR4_VME_BIT)
#define CR4_PVI             (1UL<<CR4_PVI_BIT)
#define CR4_TSD             (1UL<<CR4_TSD_BIT)
#define CR4_DE              (1UL<<CR4_DE_BIT)
#define CR4_PSE             (1UL<<CR4_PSE_BIT)
#define CR4_PAE             (1UL<<CR4_PAE_BIT)
#define CR4_MCE             (1UL<<CR4_MCE_BIT)
#define CR4_PGE             (1UL<<CR4_PGE_BIT)
#define CR4_PCE             (1UL<<CR4_PCE_BIT)
#define CR4_OSFXSR          (1UL<<CR4_OSFXSR_BIT)
#define CR4_OSXMMEXCPT      (1UL<<CR4_OSXMMEXCPT_BIT)
#define CR4_VMXE            (1UL<<CR4_VMXE_BIT)
#define CR4_SMXE            (1UL<<CR4_SMXE_BIT)
#define CR4_PCIDE           (1UL<<CR4_PCIDE_BIT)
#define CR4_OSXSAVE         (1UL<<CR4_OSXSAVE_BIT)
#define CR4_SMEP            (1UL<<CR4_SMEP_BIT)

typedef union control_register_4
{
   struct
   {
      uint32_t    vme:1;        /* virtual 8086 */
      uint32_t    pvi:1;        /* pmode virtual int */
      uint32_t    tsd:1;        /* time stamp disable */
      uint32_t    de:1;         /* debug ext */
      uint32_t    pse:1;        /* page sz ext */
      uint32_t    pae:1;        /* phys addr ext */
      uint32_t    mce:1;        /* machine check enable */
      uint32_t    pge:1;        /* page global enable */
      uint32_t    pce:1;        /* perf counter enable */
      uint32_t    osfxsr:1;     /* fxsave fxstore */
      uint32_t    osxmmexcpt:1; /* simd fpu excpt */
      uint32_t    r1:2;         /* reserved */
      uint32_t    vmxe:1;       /* vmx enable */
      uint32_t    smxe:1;       /* smx enable */
      uint32_t    r2:2;         /* reserved */
      uint32_t    pcide:1;      /* process context id */
      uint32_t    osxsave:1;    /* xsave enable */
      uint32_t    r3:1;         /* reserved */
      uint32_t    smep:1;       /* smep */
      uint32_t    r4:11;        /* smep reserved */

   } __attribute__((packed));

   raw32_t;

} __attribute__((packed)) cr4_reg_t;


/*
** Mov to/from cr (32 bits):
*/
#define get_cr(_n_)                                                     \
   ({                                                                   \
      uint32_t x;                                                       \
      asm volatile("mov %%cr" #_n_ ", %0":"=r"(x));                     \
      x;                                                                \
   })

#define set_cr(_n_,_x_) asm volatile("mov %0, %%cr" #_n_ ::"r"(_x_))

#define get_cr0()       get_cr(0)
#define get_cr2()       get_cr(2)
#define get_cr3()       get_cr(3)
#define get_cr4()       get_cr(4)

#define set_cr0(x)      set_cr(0,x)
#define set_cr2(x)      set_cr(2,x)
#define set_cr3(x)      set_cr(3,x)
#define set_cr4(x)      set_cr(4,x)

#endif
