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

block* read_cache_block(hwaddr_t _addr) {
	cache_addr* addr =  (void *)&_addr;
	uint32_t tag = addr->tag;
	uint32_t index = addr->index;
	uint32_t offs = addr->offs;
	printf("addr=0x%x,tag=0x%x,index=0x%x,offs=0x%x\n",
		*(unsigned int*)addr,tag,index,offs);
	cache_group* group = &cache[index];
	
	int i, empty_line = -1;
	for(i=0,empty_line = 0; i<ASSOCT_WAY; ++i) {
		if(group->valid_bit[i]){
			if(group->tag[i] == tag) {
				return &(group->data[i]);
			}
		}
		else
			empty_line = i;
	}
	//not found
	//TODO: check victim line
	if(empty_line < 0) {	//no empty line, create empty line
		empty_line = get_rand(ASSOCT_WAY);
	}
	//read into cache
	
	return &(group->data[empty_line]);
}


