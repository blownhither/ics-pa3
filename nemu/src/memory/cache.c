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

#ifdef MZYDEBUG
	printf("_block:addr=0x%x,tag=0x%x,\n\tindex=0x%x,offs=0x%x,addr_align=0x%x\n",*(unsigned int*)addr,tag,index,offs,addr_aligned);
#endif
	cache_group* group = &cache[index];
	block *ret_block = NULL;
	int i, empty_line;
	cache_access++;
	for(i=0, empty_line = -1; i<ASSOCT_WAY; ++i) {
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
	if(ret_block == NULL){		//not found
		cache_miss++;
		if(empty_line < 0) {	//no empty line, create empty line
			empty_line = get_rand(ASSOCT_WAY);
			//TODO: write back
		}
		//read into cache
		for(i=0; i<BLOCK_SIZE; ++i) {
			group->data[empty_line][i] = dram_read(addr_aligned + i, 1) & 0xff;	//see memory.c
#ifdef MZYDEBUG
			printf("%x ",group->data[empty_line][i]);
#endif
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
	if(offs+len >= BLOCK_SIZE) {	//unaligned read
#ifdef MZYDEBUG
		printf("in unaligned\n");
#endif
		cache_block_read(addr + len, buf + BLOCK_SIZE);
	}
	return unalign_rw((buf + offs), 4);
}

void cache_write_mask (hwaddr_t _addr, uint8_t buf[], uint8_t mask[], bool unalign_flag, size_t len){	//write under guarantee that no unaligned would happen

	cache_addr* addr =  (void *)&_addr;					//parsing addr
	uint32_t tag = addr->tag;
	uint32_t index = addr->index;
	//uint32_t offs = addr->offs;
	//uint32_t addr_aligned = *(uint32_t *)addr - offs;

#ifdef MZYDEBUG
	printf("_write_mask:addr=0x%x,tag=0x%x,\n\tindex=0x%x,offs=0x%x,addr_align=0x%x\n",*(unsigned int*)addr,tag,index,offs,addr_aligned);
#endif
	cache_group* group = &cache[index];
	block* tar_block = NULL;
	int i;
//	int empty_line = -1;
find_tar_lable:
	for(i=0; i<ASSOCT_WAY; ++i) {
		if(group->valid_bit[i]){
			if(group->tag[i] == tag) {
				tar_block = &group->data[i];
				break;
			}
		}
//		else
//			empty_line = i;
	}	
	if(tar_block == NULL) {
		cache_read(_addr, len);	//force get block
		goto find_tar_lable;
	}
	//now must have tar_block
	assert(tar_block != NULL);
	int max = (unalign_flag)?BLOCK_SIZE*2 : BLOCK_SIZE;
	for(i=0; i<max; ++i){
		if(mask[i])
			*tar_block[i] = buf[i];
	}
}

void cache_write ( hwaddr_t _addr, size_t len, uint32_t data ) {
	uint32_t offs = _addr&(BLOCK_SIZE - 1);
	uint8_t buf[2 * BLOCK_SIZE], mask[2 * BLOCK_SIZE];
	uint32_t *pos = (uint32_t *)(buf + offs);
	*pos = data;
	memset(mask, 0, sizeof(mask));
	pos = (uint32_t *)(mask + offs);
	*pos = ~0u >> ((4-len) << 8);
	cache_write_mask(_addr, buf, mask, (offs+len >= BLOCK_SIZE), len);
}

