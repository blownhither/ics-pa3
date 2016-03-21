#include "cache.h"
#include "common.h"


typedef unsigned char bool;
typedef unsigned char uint8_t;
typedef uint8_t block[BLOCK_SIZE];

typedef struct{
	bool valid_bit[ASSOCT_WAY];
	uint32_t tag[ASSOCT_WAY];
	block data[ASSOCT_WAY];
} cache_group;

cache_group cache[GROUP_NUM];

block* read_cache_block(hwaddr_t addr, size_t len) {
	uint32_t tag = (addr >> (BLOCK_LEN + INDEX_LEN)) & ~(0xffffffff << TAG_LEN);
	uint32_t index = (addr >> BLOCK_LEN) & ~(0xffffffff << INDEX_LEN);
	uint32_t offs = addr & ~( 0xffffffff << BLOCK_LEN );
	printf("addr=0x%x,tag=0x%x,index=0x%x,offs=0x%x\n",
		*(uint32_t *)(void *)&addr,tag,index,offs);
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
		
	}
	//read into cache
	return NULL;
}


