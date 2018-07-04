/* GPLv2 (c) Airbus */
#ifndef __GPR_H__
#define __GPR_H__

#include <types.h>

/*
** EFLAGS
*/
#define EFLAGS_CF_BIT         0
#define EFLAGS_PF_BIT         2
#define EFLAGS_AF_BIT         4
#define EFLAGS_ZF_BIT         6
#define EFLAGS_SF_BIT         7
#define EFLAGS_TF_BIT         8
#define EFLAGS_IF_BIT         9
#define EFLAGS_DF_BIT        10
#define EFLAGS_OF_BIT        11
#define EFLAGS_VM_BIT        17
#define EFLAGS_VIF_BIT       19
#define EFLAGS_VIP_BIT       20

#define EFLAGS_CF            (1<<EFLAGS_CF_BIT)
#define EFLAGS_PF            (1<<EFLAGS_PF_BIT)
#define EFLAGS_AF            (1<<EFLAGS_AF_BIT)
#define EFLAGS_ZF            (1<<EFLAGS_ZF_BIT)
#define EFLAGS_SF            (1<<EFLAGS_SF_BIT)
#define EFLAGS_TF            (1<<EFLAGS_TF_BIT)
#define EFLAGS_IF            (1<<EFLAGS_IF_BIT)
#define EFLAGS_DF            (1<<EFLAGS_DF_BIT)
#define EFLAGS_OF            (1<<EFLAGS_OF_BIT)
#define EFLAGS_VM            (1<<EFLAGS_VM_BIT)
#define EFLAGS_VIF           (1<<EFLAGS_VIF_BIT)
#define EFLAGS_VIP           (1<<EFLAGS_VIP_BIT)

#define EFLAGS_IOPL          (3<<12)

typedef struct cpu_eflags_register_fields
{
   uint32_t   cf:1;     /* carry */
   uint32_t   r1:1;     /* (1) */
   uint32_t   pf:1;     /* parity */
   uint32_t   r2:1;     /* (0) */
   uint32_t   af:1;     /* adjust */
   uint32_t   r3:1;     /* (0) */
   uint32_t   zf:1;     /* zero */
   uint32_t   sf:1;     /* sign */

   uint32_t   tf:1;     /* trap */
   uint32_t   it:1;     /* interrupt */
   uint32_t   df:1;     /* div */
   uint32_t   of:1;     /* overflow */
   uint32_t   iopl:2;   /* io pvl */
   uint32_t   nt:1;     /* nested task */
   uint32_t   r4:1;     /* (0) */

   uint32_t   rf:1;     /* resume */
   uint32_t   vm:1;     /* virtual 8086 */
   uint32_t   ac:1;     /* align */
   uint32_t   vit:1;    /* virtual int */

   uint32_t   vip:1;    /* virtual int pending */
   uint32_t   id:1;     /* identification */
   uint32_t   r5:10;    /* (0) */

} __attribute__((packed)) eflags_reg_fields_t;

typedef union cpu_eflags_register
{
   eflags_reg_fields_t;
   raw32_t;

} __attribute__((packed)) eflags_reg_t;

/*
** Save/Restore cpu flags (32/64)
*/
#define save_flags(flags)                               \
   asm volatile( "pushf;pop %0":"=m"(flags)::"memory" )
#define load_flags(flags)                                       \
   asm volatile( "push %0;popf"::"m"(flags):"memory","cc" )
#define clear_flags()        asm volatile( "push $0 ; popf":::"cc" );
#define get_flags()          ({ulong_t flg; save_flags(flg); flg;})

/*
** General Purpose Registers
*/
typedef union general_purpose_registers
{
   struct
   {
      raw32_t   eax;
      raw32_t   ecx;
      raw32_t   edx;
      raw32_t   ebx;
      raw32_t   esp;
      raw32_t   ebp;
      raw32_t   esi;
      raw32_t   edi;

   } __attribute__((packed));

   raw32_t      raw[8];

} __attribute__((packed)) gpr_t;

/*
** GPRs context after "pusha"
*/
typedef union general_purpose_registers_context
{
   struct
   {
      raw32_t   edi;
      raw32_t   esi;
      raw32_t   ebp;
      raw32_t   esp;
      raw32_t   ebx;
      raw32_t   edx;
      raw32_t   ecx;
      raw32_t   eax;

   } __attribute__((packed));

   raw32_t      raw[8];

} __attribute__((packed)) gpr_ctx_t;

/*
** General Purpose Register operations
*/
#define get_pc()                                                        \
   ({                                                                   \
      offset_t x;                                                       \
      asm volatile ("call 1f ; 1:pop %%eax":"=a"(x));                   \
      x;                                                                \
   })

#define get_reg(_r_)                                                    \
   ({                                                                   \
      uint32_t v;                                                       \
      asm volatile( "mov %%"#_r_", %0":"=m"(v)::"memory" );             \
      v;                                                                \
   })

#define get_ebp()         get_reg(ebp)
#define get_esp()         get_reg(esp)

#define set_reg(_r_,_v_)                                \
   asm volatile ("mov %0, %%"#_r_::"m"(_v_):"memory")

#define set_edi(val)      set_reg(edi,val)
#define set_ebp(val)      set_reg(ebp,val)
#define set_esp(val)      set_reg(esp,val)
#define set_rbp(val)      set_reg(rbp,val)
#define set_rsp(val)      set_reg(rsp,val)

#endif
