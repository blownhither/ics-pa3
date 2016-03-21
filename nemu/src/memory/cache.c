#include "cache.h"
#include "common.h"

typedef unsigned char bool;
typedef unsigned char uint8_t;
typedef uint8_t block[BLOCK_SIZE];

typedef struct{
	bool valid_bit[ASSOCT_WAY];
	uint32_t tag[ASSOCT_WAY];
	block data[ASSOCT_WAY][BLOCK_SIZE];
} cache_group;

cache_group cache[GROUP_NUM];


