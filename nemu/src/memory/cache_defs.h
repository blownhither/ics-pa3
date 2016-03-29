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
	//no dirty bit
	uint32_t tag[ASSOCT_WAY];
	block data[ASSOCT_WAY];
} L1_cache_group;

extern uint32_t dram_read(hwaddr_t addr, size_t len);
extern void dram_write(hwaddr_t addr, size_t len, uint32_t data);

static inline int get_rand(int max) {
	static bool initialized = false;
	if(!initialized) {
		initialized = true;
		srand((unsigned int) time(0));
	}
	return rand()%max;
}
