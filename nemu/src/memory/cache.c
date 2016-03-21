typedef unsigned char bool;
typedef unsigned char uint8_t;


#ifndef BLOCK_SIZE
#define BLOCK_SIZE 64
#endif
#ifndef ASSOCT_WAY
#define ASSOCT_WAY 8
#endif
#ifndef BLOCK_NUM
#define BLOCK_NUM 1024
#endif

#define LINE_SIZE BLOCK_SIZE/ASSOCT_WAY
#if LINE_SIZE<=8
#define LINE_TYPE uint8_t
#elif LINE_SIZE<=16
#define LINE_SIZE uint16_t
#else
#define LINE_SIZE uint32_t
#endif

typedef struct{
	LINE_TYPE line[LINE_SIZE];
	bool valid_bit[ASSOCT_WAY];
} cache_block;

cache_block cache[BLOCK_NUM];
