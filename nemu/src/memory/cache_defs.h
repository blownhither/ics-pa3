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
	bool dirty_bit[ASSOCT_WAY];
	uint32_t tag[ASSOCT_WAY];
	block data[ASSOCT_WAY];
} L2_cache_group;

typedef struct{
	bool valid_bit[ASSOCT_WAY];
	uint32_t tag[ASSOCT_WAY];
	block data[ASSOCT_WAY];
} cache_group;
