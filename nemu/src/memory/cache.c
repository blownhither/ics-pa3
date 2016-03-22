#include "cache.h"
#include "common.h"


typedef unsigned char bool;
typedef unsigned char uint8_t;
typedef uint8_t block[BLOCK_SIZE];

typedef struct{
	uint32_t offs 	: BLOCK_LEN; 
	uint32_t index 	: INDEX_LEN;
	uint32_t tag 	: TAG_LEN;
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
	return rand()%ASSOCT_WAY;
}

void init_cache() {
	memset(cache, 0, sizeof(cache));
}

uint64_t cache_miss = 0, cache_access = 0;
uint64_t get_cache_cost (){ 
	//printf("in get_:%x, %x\n", (int)cache_access%0xffff, (int)cache_miss&0xffff);
	return cache_access * 2 + cache_miss * 198;
}

extern uint32_t dram_read(hwaddr_t addr, size_t len);

void cache_block_read(hwaddr_t _addr, uint8_t buf[]) {
	cache_addr* addr =  (void *)&_addr;					//parsing addr
	uint32_t tag = addr->tag;
	uint32_t index = addr->index;
	uint32_t offs = addr->offs;
	uint32_t addr_aligned = *(uint32_t *)addr - offs;


	
	//printf("_block:addr=0x%x,tag=0x%x,\n\tindex=0x%x,offs=0x%x,addr_align=0x%x\n",*(unsigned int*)addr,tag,index,offs,addr_aligned);
	cache_group* group = &cache[index];
	block *ret_block = NULL;
	int i, empty_line = -1;
	cache_access++;
	for(i=0,empty_line = 0; i<ASSOCT_WAY; ++i) {
		if(group->valid_bit[i]){
			if(group->tag[i] == tag) {
				ret_block = &group->data[i];
				break;
			}
		}
		else
			empty_line = i;
	}		
	//TODO: check victim line
	
	if(ret_block == NULL){	//not found
		cache_miss++;
		if(empty_line < 0) {	//no empty line, create empty line
			empty_line = get_rand(ASSOCT_WAY);
		}
		//read into cache
		for(i=0; i<BLOCK_SIZE; ++i) {
			group->data[empty_line][i] = dram_read(addr_aligned + i, 1) & 0xff;	//see memory.c
			//printf("%x ",group->data[empty_line][i]);
			group->tag[empty_line] = tag;
			group->valid_bit[empty_line] = true;
		}
		ret_block = &group->data[empty_line];
	}
	for(i=0; i<BLOCK_SIZE; ++i) {
		buf[i] = (* ret_block)[i];
	}
}

uint32_t cache_read(hwaddr_t addr, size_t len) {
	uint8_t buf[ BLOCK_SIZE<<1 ];
	cache_block_read(addr, buf);
	uint32_t offs = addr&(BLOCK_SIZE - 1);
	if(((addr + len)&(BLOCK_SIZE - 1)) < (len - 1)) {	//unaligned read
		cache_block_read(addr + len, buf + BLOCK_SIZE);
	}
	return unalign_rw((buf + offs), 4);
}

