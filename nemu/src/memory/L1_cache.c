#include "common.h"
#include "L1_cache.h"
#include "cache_defs.h"

uint64_t L1_cache_miss = 0, L1_cache_access = 0;
uint64_t L1_get_cache_cost (){ 
	return L1_cache_access * 2;	//if miss, goto L2 cache
}

//#define MZYDEBUG

static L1_cache_group L1_cache[GROUP_NUM];	//cache[GROUP_NUM]

void init_L1_cache() {
	//L1_cache = malloc(sizeof(L1_cache_group)*GROUP_NUM);
	memset(L1_cache, 0, sizeof(L1_cache_group)*GROUP_NUM);
}



void L1_cache_block_read(hwaddr_t _addr, uint8_t buf[]) {
	cache_addr addr;
	addr.addr = _addr;
	uint32_t tag = addr.tag, index = addr.index;//, offs = addr.offs;
	//uint32_t addr_aligned = addr.addr - offs;
#ifdef MZYDEBUG
	printf("_block:addr=0x%x,tag=0x%x,\n\tindex=0x%x,offs=0x%x,addr_align=0x%x\n",*(int *)(void *)&addr,tag,index,offs,addr_aligned);
#endif
	L1_cache_group* group = &(L1_cache[index]);
	//block *ret_block = NULL;
	pblock ret_block = NULL;
	int i, empty_line;
	L1_cache_access++;
	for(i=0, empty_line = -1; i<ASSOCT_WAY; ++i) {
		if(group->valid_bit[i]){
			if(group->tag[i] == tag) {
				ret_block = group->data[i];
				break;
			}
		}
		else
			empty_line = i;
	}		
	if(ret_block == NULL){		//not found
		L1_cache_miss++;
		if(empty_line < 0) {	//no empty line, choose one, no need to write back
			empty_line = get_rand(ASSOCT_WAY);
		}
		//read into cache
		L2_cache_block_read(_addr, buf);
		memcpy(group->data[empty_line], buf, BLOCK_SIZE);
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

uint32_t L1_cache_read(hwaddr_t addr, size_t len) {
	uint8_t buf[ BLOCK_SIZE<<1 ];
	//uint8_t* buf = (uint8_t *)malloc(BLOCK_SIZE<<1);
	L1_cache_block_read(addr, buf);
	uint32_t offs = addr&(BLOCK_SIZE - 1);
	if(offs+len > BLOCK_SIZE) {	//unaligned read
		L1_cache_block_read(addr + len, buf + BLOCK_SIZE);
	}
	return unalign_rw((buf + offs), 4);
}

void 
L1_cache_write_mask (hwaddr_t _addr, uint8_t buf[], uint8_t mask[], size_t len)
{	//write under guarantee that no unaligned would happen
	cache_addr addr;
	addr.addr = _addr;
	uint32_t tag = addr.tag;
	uint32_t index = addr.index;
	L1_cache_group* group = &L1_cache[index];
	pblock tar_block = NULL;
	int i;
	for(i=0; i<ASSOCT_WAY; ++i) {
		if(group->valid_bit[i]){
			if(group->tag[i] == tag) {
				tar_block = group->data[i];	
				break;
			}
		}
	}	
	if(tar_block == NULL) {	//write miss
		L2_cache_write_mask(_addr, buf, mask, len);
		return;
	}
	//else: write hit
	assert(tar_block != NULL);
	for(i=0; i<BLOCK_SIZE; ++i){
		if(mask[i]){
			tar_block[i] = buf[i];
		}
	}
}

void L1_cache_write ( hwaddr_t _addr, size_t len, uint32_t data ) {
	uint32_t offs = _addr&(BLOCK_SIZE - 1);
	uint8_t buf[2 * BLOCK_SIZE], mask[2 * BLOCK_SIZE];
	*(uint32_t *)(buf + offs) = data;
	memset(mask, 0, 2*BLOCK_SIZE);
	memset(mask+offs, 1, len);
	L1_cache_write_mask(_addr, buf, mask, len);
	if(offs+len > BLOCK_SIZE)
		L1_cache_write_mask(_addr+len, buf+BLOCK_SIZE, mask+BLOCK_SIZE, len);
	L2_cache_write(_addr, len, data);
}

bool L1_check_cache_addr (hwaddr_t _addr){
	if(_addr >= (1<<27)){
		printf("physical address %x is outside of the physical memory!",_addr);
		return false;
	}
	cache_addr addr;
	addr.addr = _addr;
	uint32_t tag = addr.tag, index = addr.index;//, offs = addr.offs;
	//uint32_t addr_aligned = addr.addr - offs;
	L1_cache_group* group = &(L1_cache[index]);
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
	printf("L1 cache:\naddress:0x%x\tindex:0x%2x\ttag:0x%2x\t",addr.addr, addr.index, addr.tag);
	if(ret_block == NULL){
		printf("\n\tRAM block is not in cache.\n");
		return false;
	}
	printf("line num:%d",i);
	for(i=0; i<BLOCK_SIZE; ++i){
		if(!(i&0xf))printf("\n\t");
		printf("%02x ",ret_block[i]);
	}
	printf("\n");
	return true;
}

void check_cache_block(){
	panic("implement if you want");
}
