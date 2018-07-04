/* GPLv2 (c) Airbus */
#include <intr.h>
#include <cr.h>
#include <debug.h>
#include <info.h>

extern info_t *info;

static char* exception_names[] = {
   "Divide by zero",
   "Debug",
   "NMI interrupt",
   "Breakpoint",
   "Overflow",
   "BOUND range exceeded",
   "Invalid Opcode",
   "Device not available",
   "Double fault",
   "Coprocessor segment overrun",
   "Invalid TSS",
   "Segment not present",
   "Stack fault",
   "General protection",
   "Page fault",
   "Intel Reserved",
   "Floating point error",
   "Alignment check",
   "Machine check",
   "SIMD floating point"
};

void __regparm__(1) excp_hdlr(int_ctx_t *ctx)
{
   debug("\nException: %s\n", exception_names[ctx->nr.blow]);

   switch(ctx->nr.blow)
   {
   case NMI_EXCP:
      debug("#NMI (ignored)\n");
      return;

   case PF_EXCP:
   {
      debug("#PF details: p:%d wr:%d us:%d id:%d addr 0x%x\n"
            ,ctx->err.pf.p
            ,ctx->err.pf.wr
            ,ctx->err.pf.us
            ,ctx->err.pf.id
            ,get_cr2());
      break;
   }
   case GP_EXCP:
      debug("#GP details: ext:%d idt:%d ti:%d index:%d\n"
            ,ctx->err.sl.ext
            ,ctx->err.sl.idt
            ,ctx->err.sl.ti
            ,ctx->err.sl.idx);
      break;
   }

   debug("cr0 = %p\n", get_cr0());
   debug("cr4 = %p\n", get_cr4());
   stack_trace(ctx->gpr.ebp.raw);
   panic("fatal exception !\n");
}
