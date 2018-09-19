/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp()
{
  multiboot_memory_map_t *start;
	multiboot_memory_map_t *end;
	
	debug("kernel mem [0x%x - 0x%x]\n", &__kernel_start__, &__kernel_end__);
	debug("MBI flags 0x%x\n", info -> mbi -> flags);
   
	start = (multiboot_memory_map_t *) info -> mbi -> mmap_addr;
	end = (multiboot_memory_map_t *) ( info -> mbi -> mmap_addr + info -> mbi -> mmap_length);
    
  while (start < end) { 
		debug("0x%llx - 0x%llx %d\n", start -> addr, start -> len, start -> type);
		start++;
	}

}
