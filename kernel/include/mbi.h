/* GPLv2 (c) Airbus */
#ifndef __MULTIBOOT_H__
#define __MULTIBOOT_H__

#include <grub_mbi.h>
#include <types.h>

typedef multiboot_info_t        mbi_t;
typedef multiboot_module_t      module_t;
typedef multiboot_memory_map_t  memory_map_t;

#define GRUB_STR                0x42555247
#define MBI_FLAG_VER            (1<<31)

#define MBI_FLAG_BLDR           MULTIBOOT_INFO_BOOT_LOADER_NAME
#define MBI_FLAG_MEM            MULTIBOOT_INFO_MEMORY
#define MBI_FLAG_BDEV           MULTIBOOT_INFO_BOOTDEV
#define MBI_FLAG_CMDLINE        MULTIBOOT_INFO_CMDLINE
#define MBI_FLAG_MODS           MULTIBOOT_INFO_MODS
#define MBI_FLAG_MMAP           MULTIBOOT_INFO_MEM_MAP

#define MBH_MAGIC               MULTIBOOT_HEADER_MAGIC
#define MBH_FLAGS               (MBI_FLAG_MEM|MBI_FLAG_BDEV)

#define __mbh__                 __attribute__ ((section(".mbh"),aligned(4)))

/*
** Functions
*/
typedef int (*mbi_opt_hdl_t)(char*, void*);

void mbi_check_boot_loader(mbi_t*);
int  mbi_get_opt(mbi_t*, module_t*, char*, mbi_opt_hdl_t, void*);

#endif

