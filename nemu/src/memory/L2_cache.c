#include "common.h"
#include "L2_cache.h"				//config
#include "cache_defs.h"				//struct

L2_cache_group *L2_cache;	//L2_cache[GROUP_NUM]

void init_L2_cache() {
	L2_cache = malloc(sizeof(L2_cache_group)*GROUP_NUM);
	memset(L2_cache, 0, sizeof(L2_cache_group)*GROUP_NUM);
}

uint64_t L2_cache_miss = 0, L2_cache_access = 0;

uint64_t L2_get_cache_cost (){ 
	return L2_cache_access * 2 + L2_cache_miss * 198;
}

void L2_write_back_block(uint32_t index, uint32_t tag, block bk) {
	cache_addr _addr;
	_addr.tag = tag;	
	_addr.index = index;	
	_addr.offs = 0;
	int i;
	//TODO:
	for(i=0; i<BLOCK_SIZE; ++i) {
		dram_write(_addr.addr + i, 1, bk[i]);
	}
}

void L2_cache_block_read(hwaddr_t _addr, uint8_t buf[]) {
	cache_addr addr;
	addr.addr = _addr;
	uint32_t tag = addr.tag, index = addr.index, offs = addr.offs;
	uint32_t addr_aligned = addr.addr - offs;
#ifdef MZYDEBUG
	printf("_block:addr=0x%x,tag=0x%x,\n\tindex=0x%x,offs=0x%x,addr_align=0x%x\n",*(int *)(void *)&addr,tag,index,offs,addr_aligned);
#endif
	L2_cache_group* group = &(L2_cache[index]);
	pblock ret_block = NULL;
	int i, empty_line;
	L2_cache_access++;
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
		L2_cache_miss++;
#ifdef MZYDEBUG
		printf("cache_miss\n");
#endif
		if(empty_line < 0) {	//no empty line, choose one to write back
			empty_line = get_rand(ASSOCT_WAY);
			if(group->dirty_bit[empty_line])
				L2_write_back_block(index, group->tag[empty_line], group->data[empty_line]);
			//TODO: write victim line
		}
		//read into cache
		for(i=0; i<BLOCK_SIZE; ++i) {
			group->data[empty_line][i] = dram_read(addr_aligned + i, 1) & 0xff;	//see memory.c
#ifdef MZYDEBUG
			printf("%x ",group->data[empty_line][i]);
#endif
		}
		group->tag[empty_line] = tag;
		group->valid_bit[empty_line] = true;
		ret_block = group->data[empty_line];
	}
	for(i=0; i<BLOCK_SIZE; ++i) {
		buf[i] = ret_block[i];
	}
}

uint32_t L2_cache_read(hwaddr_t addr, size_t len) {
	uint8_t buf[ BLOCK_SIZE<<1 ];
	L2_cache_block_read(addr, buf);
	uint32_t offs = addr&(BLOCK_SIZE - 1);
	if(offs+len > BLOCK_SIZE) {	//unaligned read
#ifdef MZYDEBUG
		printf("in unaligned\n");
#endif
		L2_cache_block_read(addr + len, buf + BLOCK_SIZE);
	}
	return unalign_rw((buf + offs), 4);
}

void L2_cache_write_mask 
(hwaddr_t _addr, uint8_t buf[], uint8_t mask[], size_t len)
{
	//! guarantee that unalign not happen
	cache_addr addr;
	addr.addr = _addr;
	uint32_t tag = addr.tag, index = addr.index;
#ifdef MZYDEBUG
	uint32_t offs = addr.offs;
	uint32_t addr_aligned = addr.addr - offs;
	printf("_write_mask:addr=0x%x,tag=0x%x,\n\tindex=0x%x,offs=0x%x,addr_align=0x%x\n",*(unsigned int*)(void *)&addr,tag,index,offs,addr_aligned);
#endif
	L2_cache_group* group = &L2_cache[index];
	pblock tar_block = NULL;
	int i;
find_tar_lable:
	for(i=0; i<ASSOCT_WAY; ++i) {
		if(group->valid_bit[i]){
			if(group->tag[i] == tag) {
				tar_block = group->data[i];
				L2_cache_access++;
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
		L2_cache_read(_addr, len);	//force get block
		goto find_tar_lable;
	}
	//now must have tar_block
	assert(tar_block != NULL);
//	if(unalign_flag) {
//		max = 2 * BLOCK_SIZE;
//		cache_read(_addr + len, len);
//	}
	for(i=0; i<BLOCK_SIZE; ++i){
		if(mask[i]){
			tar_block[i] = buf[i];
#ifdef MZYDEBUG
			printf("%d:%x ", i, tar_block[i]);
#endif
		}
	}
}

void L2_cache_write ( hwaddr_t _addr, size_t len, uint32_t data ) {
	uint32_t offs = _addr&(BLOCK_SIZE - 1);
	uint8_t buf[2 * BLOCK_SIZE], mask[2 * BLOCK_SIZE];
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
	L2_cache_write_mask(_addr, buf, mask, len);
	if(offs+len > BLOCK_SIZE)
		L2_cache_write_mask(_addr+len, buf+BLOCK_SIZE, mask+BLOCK_SIZE, len);
}

bool L2_check_cache_addr (hwaddr_t _addr){
	if(_addr >= (1<<27)){
		printf("physical address %x is outside of the physical memory!",_addr);
		return false;
	}
	cache_addr addr;
	addr.addr = _addr;
	uint32_t tag = addr.tag, index = addr.index;
	L2_cache_group* group = &(L2_cache[index]);
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
