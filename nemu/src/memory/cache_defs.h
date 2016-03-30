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

 uint32_t dram_read(hwaddr_t addr, size_t len);
 void dram_write(hwaddr_t addr, size_t len, uint32_t data);
 void L2_cache_write_mask 
(hwaddr_t _addr, uint8_t buf[], uint8_t mask[], size_t len);
 void L2_cache_write ( hwaddr_t _addr, size_t len, uint32_t data );
 void L2_cache_block_read(hwaddr_t _addr, uint8_t buf[]);
 uint32_t L2_cache_read(hwaddr_t addr, size_t len);
 void L1_cache_write ( hwaddr_t _addr, size_t len, uint32_t data );
 uint32_t L1_cache_read(hwaddr_t addr, size_t len);


static inline int get_rand(int max) {
	static bool initialized = false;
	if(!initialized) {
		initialized = true;
		srand((unsigned int) time(0));
	}
	return rand()%max;
}
