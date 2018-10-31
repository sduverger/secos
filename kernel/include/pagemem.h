/* GPLv2 (c) Airbus */
#ifndef __PAGEMEM_H__
#define __PAGEMEM_H__

#include <types.h>

/*
** Paging bits accessing
*/
#define PG_P_BIT        0
#define PG_RW_BIT       1
#define PG_LVL_BIT      2
#define PG_PWT_BIT      3
#define PG_PCD_BIT      4
#define PG_ACC_BIT      5
#define PG_DRT_BIT      6
#define PG_PAT_BIT      7
#define PG_GLB_BIT      8

#define PG_P            (1<<PG_P_BIT)
#define PG_RO           0
#define PG_RW           (1<<PG_RW_BIT)
#define PG_KRN          0
#define PG_USR          (1<<PG_LVL_BIT)
#define PG_PWT          (1<<PG_PWT_BIT)
#define PG_PCD          (1<<PG_PCD_BIT)
#define PG_ACC          (1<<PG_ACC_BIT)
#define PG_DRT          (1<<PG_DRT_BIT)
#define PG_PAT          (1<<PG_PAT_BIT)
#define PG_GLB          (1<<PG_GLB_BIT)

/*
** Large pages
*/
#define PG_PS_BIT       7
#define PG_LPAT_BIT     12

#define PG_PS           (1<<PG_PS_BIT)
#define PG_LPAT         (1<<PG_LPAT_BIT)

typedef union page_directory_entry
{
   struct
   {
      uint32_t  p:1;
      uint32_t  rw:1;
      uint32_t  lvl:1;
      uint32_t  pwt:1;
      uint32_t  pcd:1;
      uint32_t  acc:1;
      uint32_t  mbz:3;
      uint32_t  avl:3;
      uint32_t  addr:20;

   } __attribute__((packed));

   struct
   {
      uint32_t  p:1;
      uint32_t  rw:1;
      uint32_t  lvl:1;
      uint32_t  pwt:1;
      uint32_t  pcd:1;
      uint32_t  acc:1;
      uint32_t  d:1;
      uint32_t  ps:1;
      uint32_t  g:1;
      uint32_t  avl:3;
      uint32_t  pat:1;
      uint32_t  rsv:9;
      uint32_t  addr:10;

   } __attribute__((packed)) page;

   raw32_t;

} __attribute__((packed)) pde32_t;

typedef union page_table_entry
{
   struct
   {
      uint32_t  p:1;
      uint32_t  rw:1;
      uint32_t  lvl:1;
      uint32_t  pwt:1;
      uint32_t  pcd:1;
      uint32_t  acc:1;
      uint32_t  d:1;
      uint32_t  pat:1;
      uint32_t  g:1;
      uint32_t  avl:3;
      uint32_t  addr:20;

   } __attribute__((packed));

   raw32_t;

} __attribute__((packed)) pte32_t;

/*
** 32 bits paging usefull macros
*/
#define PDE32_PER_PD                 1024UL
#define PTE32_PER_PT                 1024UL

#define PG_4K_SHIFT                  12
#define PG_4K_SIZE                   (1UL<<PG_4K_SHIFT)
#define pg_4K_offset(addr)           ((addr)&(PG_4K_SIZE-1))
#define pg_4K_nr(addr)               ((addr)>>PG_4K_SHIFT)
#define pg_4K_addr(bits)             ((bits)<<PG_4K_SHIFT)
#define pg_4K_align(addr)            __align(addr,PG_4K_SIZE)
#define pg_4K_align_next(addr)       __align_next(addr,PG_4K_SIZE)
#define pg_4K_aligned(addr)          __aligned(addr,PG_4K_SIZE)

#define PG_4M_SHIFT                  22
#define PG_4M_SIZE                   (1UL<<PG_4M_SHIFT)
#define pg_4M_offset(addr)           ((addr)&(PG_4M_SIZE-1))
#define pg_4M_nr(addr)               ((addr)>>PG_4M_SHIFT)
#define pg_4M_addr(bits)             ((bits)<<PG_4M_SHIFT)
#define pg_4M_align(addr)            __align(addr,PG_4M_SIZE)
#define pg_4M_align_next(addr)       __align_next(addr,PG_4M_SIZE)
#define pg_4M_aligned(addr)          __aligned(addr,PG_4M_SIZE)

#define pd32_idx(addr)               ((((offset_t)addr)>>PG_4M_SHIFT)&0x3ff)
#define pt32_idx(addr)               ((((offset_t)addr)>>PG_4K_SHIFT)&0x3ff)

/*
** convenient
*/
#define PAGE_SIZE                    PG_4K_SIZE
#define PAGE_SHIFT                   PG_4K_SHIFT

#define page_align(addr)             __align(addr,PAGE_SIZE)
#define page_align_next(addr)        __align_next(addr,PAGE_SIZE)
#define page_aligned(addr)           __aligned(addr,PAGE_SIZE)

#define page_nr(addr)                pg_4K_nr((offset_t)addr)
#define page_addr(bits)              pg_4K_addr((offset_t)bits)

#define pg_present(_e_)              ((_e_)->p)
#define pg_readable(_e_)             pg_present(_e_)
#define pg_writable(_e_)             (pg_present(_e_) && ((_e_)->rw))
#define pg_large(_e_)                ((_e_)->page.ps)
#define pg_zero(_e_)                 ((_e_)->raw = 0)

#define pg_set_entry(_e_,_attr_,_pfn_)          \
   ({                                           \
      (_e_)->raw  = (_attr_)|PG_P;              \
      (_e_)->addr = _pfn_;                      \
   })

#define pg_set_large_entry(_e_,_attr_,_pfn_)    \
   ({                                           \
      (_e_)->raw       = (_attr_)|PG_PS|PG_P;   \
      (_e_)->page.addr = _pfn_;                 \
   })

#include <string.h>
#define __clear_page(_d)             _memset32(_d,  0, PAGE_SIZE)
#define __copy_page(_d,_s)           _memcpy32(_d, _s, PAGE_SIZE)

/*
** Invalidate 32 bits TLB entry
*/
#define invalidate(addr)             \
   asm volatile ("invlpg %0"::"m"(addr):"memory")


#endif
