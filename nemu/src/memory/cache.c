#include "cache.h"
#include "common.h"


typedef unsigned char bool;
typedef unsigned char uint8_t;
typedef uint8_t block[BLOCK_SIZE];

typedef struct{
	uint32_t tag : TAG_LEN;
	uint32_t index : INDEX_LEN;
	uint32_t offs : BLOCK_LEN; 
} cache_addr;

typedef struct{
	bool valid_bit[ASSOCT_WAY];
	uint32_t tag[ASSOCT_WAY];
	block data[ASSOCT_WAY];
} cache_group;

cache_group cache[GROUP_NUM];

int get_rand(int max) {
	static bool initialized = false;
	if(!initialized) {
		initialized = true;
		srand((unsigned int) time(0));
	}
	return rand();
}

void init_cache() {
	memset(cache, 0, sizeof(cache));
}

extern uint32_t dram_read(hwaddr_t addr, size_t len);

void read_cache_block(hwaddr_t _addr, uint8_t buf[]) {
	cache_addr* addr =  (void *)&_addr;
	uint32_t tag = addr->tag;
	uint32_t index = addr->index;
	uint32_t offs = addr->offs;
	uint32_t addr_aligned = *(uint32_t *)addr - offs;
	printf("_block:addr=0x%x,tag=0x%x,\n\tindex=0x%x,offs=0x%x,addr_align=0x%x\n",
		*(unsigned int*)addr,tag,index,offs,addr_aligned);
	cache_group* group = &cache[index];
	block *ret_block = NULL;
	int i, empty_line = -1;
	for(i=0,empty_line = 0; i<ASSOCT_WAY; ++i) {
		if(group->valid_bit[i]){
			if(group->tag[i] == tag) {
				ret_block = &group->data[i];
			}
		}
		else
			empty_line = i;
	}		
	//TODO: check victim line
	//not found
	if(!ret_block){
		if(empty_line < 0) {	//no empty line, create empty line
			empty_line = get_rand(ASSOCT_WAY);
		}
		//read into cache
		for(i=0; i<BLOCK_SIZE; ++i) {
			group->data[empty_line][i] = dram_read(addr_aligned, 1) & 0xff;	//see memory.c
		}
		ret_block = &group->data[empty_line];
	}
	for(i=0; i<BLOCK_SIZE; ++i) {
		buf[i] = *(ret_block[i]);
	}
}

uint32_t read_cache(hwaddr_t addr, size_t len) {
	uint8_t buf[ BLOCK_SIZE<<1 ];
	read_cache_block(addr, buf);
	if(((addr + len)&(BLOCK_SIZE - 1)) < (len - 1)) {	//unaligned read
		read_cache_block(addr + len, buf + BLOCK_SIZE);
	}
	return unalign_rw(buf + (addr&(BLOCK_SIZE - 1)), 4);
}

