#include "cache.h"
#include "common.h"

//#define MZYDEBUG

extern uint32_t dram_read(hwaddr_t addr, size_t len);
extern void dram_write(hwaddr_t addr, size_t len, uint32_t data);
typedef unsigned char bool;
typedef unsigned char uint8_t;
typedef uint8_t block[BLOCK_SIZE];
typedef uint8_t *pblock;

typedef union {
	struct{
		uint32_t offs 	: BLOCK_LEN; 
		uint32_t index 	: INDEX_LEN;
		uint32_t tag 	: TAG_LEN;
	};
	uint32_t addr;
} cache_addr;

typedef struct{
	bool valid_bit[ASSOCT_WAY];
	uint32_t tag[ASSOCT_WAY];
	block data[ASSOCT_WAY];
} cache_group;

cache_group *cache;	//cache[GROUP_NUM]

int get_rand(int max) {
	static bool initialized = false;
	if(!initialized) {
		initialized = true;
		srand((unsigned int) time(0));
	}
	return rand()%max;
}

void init_cache() {
	cache = malloc(sizeof(cache_group)*GROUP_NUM);
	memset(cache, 0, sizeof(cache_group)*GROUP_NUM);
}

uint64_t cache_miss = 0, cache_access = 0;
uint64_t get_cache_cost (){ 
	//printf("in get_:%x, %x\n", (int)cache_access%0xffff, (int)cache_miss&0xffff);
	return cache_access * 2 + cache_miss * 198;
}

void write_back_block(uint32_t index, uint32_t tag, block bk) {
	cache_addr _addr;
	_addr.tag = tag;	
	_addr.index = index;	
	_addr.offs = 0;
	int i;
	//printf("write_back_block\n");
	for(i=0; i<BLOCK_SIZE; ++i) {
		dram_write(_addr.addr + i, 1, bk[i]);
		//printf("%x ",bk[i]);
	}
	//printf("\n");
}

void cache_block_read(hwaddr_t _addr, uint8_t buf[]) {
	cache_addr addr;
	addr.addr = _addr;
	uint32_t tag = addr.tag, index = addr.index, offs = addr.offs;
	uint32_t addr_aligned = addr.addr - offs;
#ifdef MZYDEBUG
	printf("_block:addr=0x%x,tag=0x%x,\n\tindex=0x%x,offs=0x%x,addr_align=0x%x\n",*(int *)(void *)&addr,tag,index,offs,addr_aligned);
#endif
	cache_group* group = &(cache[index]);
	//block *ret_block = NULL;
	pblock ret_block = NULL;
	int i, empty_line;
	cache_access++;
	for(i=0, empty_line = -1; i<ASSOCT_WAY; ++i) {
		if(group->valid_bit[i]){
			if(group->tag[i] == tag) {
				ret_block = group->data[i];
#ifdef MZYDEBUG
				printf("cache hit in block_read with");
				int j;
				for(j=0; j<BLOCK_SIZE; ++j)
					printf("%x ",ret_block[j]);
				printf("\n");
#endif
				break;
			}
		}
		else
			empty_line = i;
	}		
	//TODO: check victim line
	if(ret_block == NULL){		//not found
		cache_miss++;
#ifdef MZYDEBUG
		printf("cache_miss\n");
#endif
		if(empty_line < 0) {	//no empty line, choose one to write back
			empty_line = get_rand(ASSOCT_WAY);
			//printf("write back\n");
			write_back_block(index, group->tag[empty_line], group->data[empty_line]);
			//TODO: write victim line
		}
		//read into cache
		for(i=0; i<BLOCK_SIZE; ++i) {
			group->data[empty_line][i] = dram_read(addr_aligned + i, 1) & 0xff;	//see memory.c
#ifdef MZYDEBUG
			printf("%x ",group->data[empty_line][i]);
#endif
		}
#ifdef MZYDEBUG
		printf("end read into cache\n");
#endif
		group->tag[empty_line] = tag;
		group->valid_bit[empty_line] = true;
		ret_block = group->data[empty_line];
	}
	for(i=0; i<BLOCK_SIZE; ++i) {
		buf[i] = ret_block[i];
#ifdef MZYDEBUG
		printf("%x ",buf[i]);
#endif
	}
}

uint32_t cache_read(hwaddr_t addr, size_t len) {
	//uint8_t buf[ BLOCK_SIZE<<1 ];
	uint8_t* buf = (uint8_t *)malloc(BLOCK_SIZE<<1);
	cache_block_read(addr, buf);
	uint32_t offs = addr&(BLOCK_SIZE - 1);
	if(offs+len > BLOCK_SIZE) {	//unaligned read
#ifdef MZYDEBUG
		printf("in unaligned\n");
#endif
		cache_block_read(addr + len, buf + BLOCK_SIZE);
	}
	return unalign_rw((buf + offs), 4);
}

void cache_write_mask (hwaddr_t _addr, uint8_t buf[], uint8_t mask[], bool unalign_flag, size_t len){	//write under guarantee that no unaligned would happen

	cache_addr addr;
	addr.addr = _addr;
	uint32_t tag = addr.tag;
	uint32_t index = addr.index;
#ifdef MZYDEBUG
	uint32_t offs = addr.offs;
	uint32_t addr_aligned = addr.addr - offs;
	printf("_write_mask:addr=0x%x,tag=0x%x,\n\tindex=0x%x,offs=0x%x,addr_align=0x%x\n",*(unsigned int*)(void *)&addr,tag,index,offs,addr_aligned);
#endif
	cache_group* group = &cache[index];
	pblock tar_block = NULL;
	int i;
find_tar_lable:
	for(i=0; i<ASSOCT_WAY; ++i) {
		if(group->valid_bit[i]){
			if(group->tag[i] == tag) {
				tar_block = group->data[i];
#ifdef MZYDEBUG				
				printf("cache hit in _write_mask\n");
#endif				
				break;
			}
		}
	}	
	if(tar_block == NULL) {
#ifdef MZYDEBUG	
		printf("miss/cache_read in write_mask: 0x%x, len=%d\n",_addr, (int)len);
#endif
		cache_read(_addr, len);	//force get block
		goto find_tar_lable;
	}
	//now must have tar_block
	assert(tar_block != NULL);
	int max = BLOCK_SIZE;
	if(unalign_flag) {
		max = 2 * BLOCK_SIZE;
		cache_read(_addr + len, len);
		//printf("in unalign\n");
	}
	for(i=0; i<max; ++i){
		if(mask[i]){
			tar_block[i] = buf[i];
#ifdef MZYDEBUG
			printf("%d:%x ", i, tar_block[i]);
#endif
		}
	}
}

void cache_write ( hwaddr_t _addr, size_t len, uint32_t data ) {
	uint32_t offs = _addr&(BLOCK_SIZE - 1);
	//uint8_t buf[2 * BLOCK_SIZE], mask[2 * BLOCK_SIZE];
	uint8_t *buf = malloc(BLOCK_SIZE * 2), *mask = malloc(BLOCK_SIZE * 2);
	*(uint32_t *)(buf + offs) = data;
	memset(mask, 0, 2*BLOCK_SIZE);
	memset(mask+offs, 1, len);
#ifdef MZYDEBUG
	printf("to write 0x%x at 0x%x\n",data,_addr);
	int i;
	for(i=0; i<2*BLOCK_SIZE; ++i) {
		if(!(i&7))printf("\n");
		printf("%x&%x ",buf[i],mask[i]);
	}
#endif
	cache_write_mask(_addr, buf, mask, (offs+len > BLOCK_SIZE), len);
}

bool check_cache_addr (hwaddr_t _addr){
	if(_addr >= (1<<27)){
		printf("physical address %x is outside of the physical memory!",_addr);
		return false;
	}
	cache_addr addr;
	addr.addr = _addr;
	uint32_t tag = addr.tag, index = addr.index;//, offs = addr.offs;
	//uint32_t addr_aligned = addr.addr - offs;
	cache_group* group = &(cache[index]);
	//block *ret_block = NULL;
	pblock ret_block = NULL;
	int i;
	for(i=0; i<ASSOCT_WAY; ++i) {
		if(group->valid_bit[i]){
			if(group->tag[i] == tag) {
				ret_block = group->data[i];
				break;
			}
		}
	}		
	printf("address:0x%x\tindex:0x%2x\ttag:0x%2x\tline num:%d",addr.addr, addr.index, addr.tag,i);
	if(ret_block == NULL){
		printf("\tRAM block is not in cache.\n");
		return false;
	}
	for(i=0; i<BLOCK_SIZE; ++i){
		if(!(i&0xf))printf("\n\t");
		printf("%2x ",ret_block[i]);
	}
	printf("\n");
	return true;
}

void check_cache_block(){
	panic("implement if you want");
}
