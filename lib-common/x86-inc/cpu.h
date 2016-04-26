#ifndef __X86_INC_CPU_H__
#define __X86_INC_CPU_H__

/* the Control Register 0 */
typedef union CR0 {
	struct {
		uint32_t protect_enable      : 1;
		uint32_t monitor_coprocessor : 1;
		uint32_t emulation           : 1;
		uint32_t task_switched       : 1;
		uint32_t extension_type      : 1;
		uint32_t numeric_error       : 1;
		uint32_t pad0                : 10;
		uint32_t write_protect       : 1; 
		uint32_t pad1                : 1; 
		uint32_t alignment_mask      : 1;
		uint32_t pad2                : 10;
		uint32_t no_write_through    : 1;
		uint32_t cache_disable       : 1;
		uint32_t paging              : 1;
	};
	
	struct {
		uint32_t PE	:1;
		uint32_t MP	:1;
		uint32_t EM	:1;
		uint32_t TS	:1;
		uint32_t ET	:1;
		uint32_t 	:26;
		uint32_t PG	:1;
	};
	
	uint32_t val;
} CR0;

/* the Control Register 3 (physical address of page directory) */
typedef union CR3 {
	struct {
		uint32_t pad0                : 3;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t pad1                : 7;
		uint32_t page_directory_base : 20;
	};
	struct {
		uint32_t _pad0                	: 3;
		uint32_t _page_write_through  	: 1;
		uint32_t _page_cache_disable	: 1;
		uint32_t _pad1             	  	: 7;
		uint32_t base 					: 20;
	};
	uint32_t val;
} CR3;

#endif
